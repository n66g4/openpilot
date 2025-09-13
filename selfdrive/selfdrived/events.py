#!/usr/bin/env python3
import bisect
import math
import os
from enum import IntEnum
from collections.abc import Callable

from cereal import log, car
import cereal.messaging as messaging
from openpilot.common.constants import CV
from openpilot.common.git import get_short_branch
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.locationd.calibrationd import MIN_SPEED_FILTER
from openpilot.system.micd import SAMPLE_RATE, SAMPLE_BUFFER
from openpilot.selfdrive.ui.feedback.feedbackd import FEEDBACK_MAX_DURATION

AlertSize = log.SelfdriveState.AlertSize
AlertStatus = log.SelfdriveState.AlertStatus
VisualAlert = car.CarControl.HUDControl.VisualAlert
AudibleAlert = car.CarControl.HUDControl.AudibleAlert
EventName = log.OnroadEvent.EventName


# Alert priorities
class Priority(IntEnum):
  LOWEST = 0
  LOWER = 1
  LOW = 2
  MID = 3
  HIGH = 4
  HIGHEST = 5


# Event types
class ET:
  ENABLE = 'enable'
  PRE_ENABLE = 'preEnable'
  OVERRIDE_LATERAL = 'overrideLateral'
  OVERRIDE_LONGITUDINAL = 'overrideLongitudinal'
  NO_ENTRY = 'noEntry'
  WARNING = 'warning'
  USER_DISABLE = 'userDisable'
  SOFT_DISABLE = 'softDisable'
  IMMEDIATE_DISABLE = 'immediateDisable'
  PERMANENT = 'permanent'


# get event name from enum
EVENT_NAME = {v: k for k, v in EventName.schema.enumerants.items()}


class Events:
  def __init__(self):
    self.events: list[int] = []
    self.static_events: list[int] = []
    self.event_counters = dict.fromkeys(EVENTS.keys(), 0)

  @property
  def names(self) -> list[int]:
    return self.events

  def __len__(self) -> int:
    return len(self.events)

  def add(self, event_name: int, static: bool=False) -> None:
    if static:
      bisect.insort(self.static_events, event_name)
    bisect.insort(self.events, event_name)

  def clear(self) -> None:
    self.event_counters = {k: (v + 1 if k in self.events else 0) for k, v in self.event_counters.items()}
    self.events = self.static_events.copy()

  def contains(self, event_type: str) -> bool:
    return any(event_type in EVENTS.get(e, {}) for e in self.events)

  def create_alerts(self, event_types: list[str], callback_args=None):
    if callback_args is None:
      callback_args = []

    ret = []
    for e in self.events:
      types = EVENTS[e].keys()
      for et in event_types:
        if et in types:
          alert = EVENTS[e][et]
          if not isinstance(alert, Alert):
            alert = alert(*callback_args)

          if DT_CTRL * (self.event_counters[e] + 1) >= alert.creation_delay:
            alert.alert_type = f"{EVENT_NAME[e]}/{et}"
            alert.event_type = et
            ret.append(alert)
    return ret

  def add_from_msg(self, events):
    for e in events:
      bisect.insort(self.events, e.name.raw)

  def to_msg(self):
    ret = []
    for event_name in self.events:
      event = log.OnroadEvent.new_message()
      event.name = event_name
      for event_type in EVENTS.get(event_name, {}):
        setattr(event, event_type, True)
      ret.append(event)
    return ret


class Alert:
  def __init__(self,
               alert_text_1: str,
               alert_text_2: str,
               alert_status: log.SelfdriveState.AlertStatus,
               alert_size: log.SelfdriveState.AlertSize,
               priority: Priority,
               visual_alert: car.CarControl.HUDControl.VisualAlert,
               audible_alert: car.CarControl.HUDControl.AudibleAlert,
               duration: float,
               creation_delay: float = 0.):

    self.alert_text_1 = alert_text_1
    self.alert_text_2 = alert_text_2
    self.alert_status = alert_status
    self.alert_size = alert_size
    self.priority = priority
    self.visual_alert = visual_alert
    self.audible_alert = audible_alert

    self.duration = int(duration / DT_CTRL)

    self.creation_delay = creation_delay

    self.alert_type = ""
    self.event_type: str | None = None

  def __str__(self) -> str:
    return f"{self.alert_text_1}/{self.alert_text_2} {self.priority} {self.visual_alert} {self.audible_alert}"

  def __gt__(self, alert2) -> bool:
    if not isinstance(alert2, Alert):
      return False
    return self.priority > alert2.priority

EmptyAlert = Alert("" , "", AlertStatus.normal, AlertSize.none, Priority.LOWEST,
                   VisualAlert.none, AudibleAlert.none, 0)

class NoEntryAlert(Alert):
  def __init__(self, alert_text_2: str,
               alert_text_1: str = "openpilot 無法使用",
               visual_alert: car.CarControl.HUDControl.VisualAlert=VisualAlert.none):
    super().__init__(alert_text_1, alert_text_2, AlertStatus.normal,
                     AlertSize.mid, Priority.LOW, visual_alert,
                     AudibleAlert.refuse, 3.)


class SoftDisableAlert(Alert):
  def __init__(self, alert_text_2: str):
    super().__init__("立即接管控制", alert_text_2,
                     AlertStatus.userPrompt, AlertSize.full,
                     Priority.MID, VisualAlert.steerRequired,
                     AudibleAlert.warningSoft, 2.),


# less harsh version of SoftDisable, where the condition is user-triggered
class UserSoftDisableAlert(SoftDisableAlert):
  def __init__(self, alert_text_2: str):
    super().__init__(alert_text_2),
    self.alert_text_1 = "openpilot 即將解除"


class ImmediateDisableAlert(Alert):
  def __init__(self, alert_text_2: str):
    super().__init__("立即接管控制", alert_text_2,
                     AlertStatus.critical, AlertSize.full,
                     Priority.HIGHEST, VisualAlert.steerRequired,
                     AudibleAlert.warningImmediate, 4.),


class EngagementAlert(Alert):
  def __init__(self, audible_alert: car.CarControl.HUDControl.AudibleAlert):
    super().__init__("", "",
                     AlertStatus.normal, AlertSize.none,
                     Priority.MID, VisualAlert.none,
                     audible_alert, .2),


class NormalPermanentAlert(Alert):
  def __init__(self, alert_text_1: str, alert_text_2: str = "", duration: float = 0.2, priority: Priority = Priority.LOWER, creation_delay: float = 0.):
    super().__init__(alert_text_1, alert_text_2,
                     AlertStatus.normal, AlertSize.mid if len(alert_text_2) else AlertSize.small,
                     priority, VisualAlert.none, AudibleAlert.none, duration, creation_delay=creation_delay),


class StartupAlert(Alert):
  def __init__(self, alert_text_1: str, alert_text_2: str = "請始終將雙手放在方向盤上並注視道路", alert_status=AlertStatus.normal):
    super().__init__(alert_text_1, alert_text_2,
                     alert_status, AlertSize.mid,
                     Priority.LOWER, VisualAlert.none, AudibleAlert.none, 5.),



# ********** helper functions **********
def get_display_speed(speed_ms: float, metric: bool) -> str:
  speed = int(round(speed_ms * (CV.MS_TO_KPH if metric else CV.MS_TO_MPH)))
  unit = 'km/h' if metric else 'mph'
  return f"{speed} {unit}"


# ********** alert callback functions **********

AlertCallbackType = Callable[[car.CarParams, car.CarState, messaging.SubMaster, bool, int, log.ControlsState], Alert]


def soft_disable_alert(alert_text_2: str) -> AlertCallbackType:
  def func(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
    if soft_disable_time < int(0.5 / DT_CTRL):
      return ImmediateDisableAlert(alert_text_2)
    return SoftDisableAlert(alert_text_2)
  return func

def user_soft_disable_alert(alert_text_2: str) -> AlertCallbackType:
  def func(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
    if soft_disable_time < int(0.5 / DT_CTRL):
      return ImmediateDisableAlert(alert_text_2)
    return UserSoftDisableAlert(alert_text_2)
  return func

def startup_master_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  branch = get_short_branch()  # Ensure get_short_branch is cached to avoid lags on startup
  if "REPLAY" in os.environ:
    branch = "replay"

  return StartupAlert("警告：此分支未經測試", branch, alert_status=AlertStatus.userPrompt)

def below_engage_speed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  return NoEntryAlert(f"車速超過 {get_display_speed(CP.minEnableSpeed, metric)} 即可啟用")


def below_steer_speed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  return Alert(
    f"轉向輔助無法使用於 {get_display_speed(CP.minSteerSpeed, metric)} 以下",
    "",
    AlertStatus.userPrompt, AlertSize.small,
    Priority.LOW, VisualAlert.steerRequired, AudibleAlert.prompt, 0.4)


def calibration_incomplete_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  first_word = '重新校準' if sm['liveCalibration'].calStatus == log.LiveCalibrationData.Status.recalibrating else '校準'
  return Alert(
    f"{first_word} 進行中：{sm['liveCalibration'].calPerc:.0f}%",
    f"駕駛速度高於 {get_display_speed(MIN_SPEED_FILTER, metric)}",
    AlertStatus.normal, AlertSize.mid,
    Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .2)


def audio_feedback_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  duration = FEEDBACK_MAX_DURATION - ((sm['audioFeedback'].blockNum + 1) * SAMPLE_BUFFER / SAMPLE_RATE)
  return NormalPermanentAlert(
    "錄製音訊回饋",
    f"{round(duration)} 秒{'s' if round(duration) != 1 else ''} 剩餘。再次按下可提前儲存。",
    priority=Priority.LOW)


# *** debug alerts ***

def out_of_space_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  full_perc = round(100. - sm['deviceState'].freeSpacePercent)
  return NormalPermanentAlert("儲存空間不足", f"{full_perc}% 已使用")


def posenet_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  mdl = sm['modelV2'].velocity.x[0] if len(sm['modelV2'].velocity.x) else math.nan
  err = CS.vEgo - mdl
  msg = f"速度誤差：{err:.1f} m/s"
  return NoEntryAlert(msg, alert_text_1="Posenet Speed Invalid")


def process_not_running_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  not_running = [p.name for p in sm['managerState'].processes if not p.running and p.shouldBeRunning]
  msg = ', '.join(not_running)
  return NoEntryAlert(msg, alert_text_1="Process Not Running")


def comm_issue_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  bs = [s for s in sm.data.keys() if not sm.all_checks([s, ])]
  msg = ', '.join(bs[:4])  # can't fit too many on one line
  return NoEntryAlert(msg, alert_text_1="Communication Issue Between Processes")


def camera_malfunction_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  all_cams = ('roadCameraState', 'driverCameraState', 'wideRoadCameraState')
  bad_cams = [s.replace('State', '') for s in all_cams if s in sm.data.keys() and not sm.all_checks([s, ])]
  return NormalPermanentAlert("攝影機故障", ', '.join(bad_cams))


def calibration_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  rpy = sm['liveCalibration'].rpyCalib
  yaw = math.degrees(rpy[2] if len(rpy) == 3 else math.nan)
  pitch = math.degrees(rpy[1] if len(rpy) == 3 else math.nan)
  angles = f"重新安裝裝置 (Pitch: {pitch:.1f}°, Yaw: {yaw:.1f}°)"
  return NormalPermanentAlert("校準無效", angles)


def paramsd_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  if not sm['liveParameters'].angleOffsetValid:
    angle_offset_deg = sm['liveParameters'].angleOffsetDeg
    title = "偵測到轉向系統未對準"
    text = f"角度偏移過高 (偏移量: {angle_offset_deg:.1f}°)"
  elif not sm['liveParameters'].steerRatioValid:
    steer_ratio = sm['liveParameters'].steerRatio
    title = "轉向比不匹配"
    text = f"轉向齒輪幾何可能偏移（比率：{steer_ratio:.1f}）"
  elif not sm['liveParameters'].stiffnessFactorValid:
    stiffness_factor = sm['liveParameters'].stiffnessFactor
    title = "Abnormal tire stiffness"
    text = f"Check tires, pressure, or alignment (Factor: {stiffness_factor:.1f})"
  else:
    return NoEntryAlert("參數暫時性錯誤")

  return NoEntryAlert(alert_text_1=title, alert_text_2=text)

def overheat_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  cpu = max(sm['deviceState'].cpuTempC, default=0.)
  gpu = max(sm['deviceState'].gpuTempC, default=0.)
  temp = max((cpu, gpu, sm['deviceState'].memoryTempC))
  return NormalPermanentAlert("系統過熱", f"{temp:.0f} °C")


def low_memory_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  return NormalPermanentAlert("記憶體不足", f"{sm['deviceState'].memoryUsagePercent}% 已使用")


def high_cpu_usage_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  x = max(sm['deviceState'].cpuUsagePercent, default=0.)
  return NormalPermanentAlert("高CPU使用率", f"{x}% 已使用")


def modeld_lagging_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  return NormalPermanentAlert("駕駛模型延遲", f"{sm['modelV2'].幀丟失率:.1f}% 幀已丟失")


def wrong_car_mode_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  text = "啟用自適應巡航功能"
  if CP.brand == "honda":
    text = "啟用主開關以啟動"
  return NoEntryAlert(text)


def joystick_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  gb = sm['carControl'].actuators.accel / 4.
  steer = sm['carControl'].actuators.torque
  vals = f"油門: {round(gb * 100.)}%, 轉向: {round(steer * 100.)}%"
  return NormalPermanentAlert("搖桿模式", vals)


def longitudinal_maneuver_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  ad = sm['alertDebug']
  audible_alert = AudibleAlert.prompt if 'Active' in ad.alertText1 else AudibleAlert.none
  alert_status = AlertStatus.userPrompt if 'Active' in ad.alertText1 else AlertStatus.normal
  alert_size = AlertSize.mid if ad.alertText2 else AlertSize.small
  return Alert(ad.alertText1, ad.alertText2,
               alert_status, alert_size,
               Priority.LOW, VisualAlert.none, audible_alert, 0.2)


def personality_changed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  personality = str(personality).title()
  return NormalPermanentAlert(f"駕駛模式：{personality}", duration=1.5)


def invalid_lkas_setting_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  text = "切換原廠LKAS開啟或關閉以啟用"
  if CP.brand == "tesla":
    text = "切換至交通感知巡航控制以啟用"
  elif CP.brand == "mazda":
    text = "啟用您車輛的車道維持輔助系統以啟動"
  elif CP.brand == "nissan":
    text = "停用原廠LKAS以啟用"
  return NormalPermanentAlert("LKAS設定無效", text)



EVENTS: dict[int, dict[str, Alert | AlertCallbackType]] = {
  # ********** events with no alerts **********

  EventName.stockFcw: {},
  EventName.actuatorsApiUnavailable: {},

  # ********** events only containing alerts displayed in all states **********

  EventName.joystickDebug: {
    ET.WARNING: joystick_alert,
    ET.PERMANENT: NormalPermanentAlert("搖桿模式"),
  },

  EventName.longitudinalManeuver: {
    ET.WARNING: longitudinal_maneuver_alert,
    ET.PERMANENT: NormalPermanentAlert("縱向操縱模式",
                                       "確保前方道路暢通"),
  },

  EventName.selfdriveInitializing: {
    ET.NO_ENTRY: NoEntryAlert("系統初始化中"),
  },

  EventName.startup: {
    ET.PERMANENT: StartupAlert("隨時準備接管控制")
  },

  EventName.startupMaster: {
    ET.PERMANENT: startup_master_alert,
  },

  EventName.startupNoControl: {
    ET.PERMANENT: StartupAlert("行車記錄器模式"),
    ET.NO_ENTRY: NoEntryAlert("行車記錄器模式"),
  },

  EventName.startupNoCar: {
    ET.PERMANENT: StartupAlert("不支援車輛的行車記錄器模式"),
  },

  EventName.startupNoSecOcKey: {
    ET.PERMANENT: NormalPermanentAlert("行車記錄器模式",
                                       "安全金鑰不可用",
                                       priority=Priority.HIGH),
  },

  EventName.dashcamMode: {
    ET.PERMANENT: NormalPermanentAlert("行車記錄模式",
                                       priority=Priority.LOWEST),
  },

  EventName.invalidLkasSetting: {
    ET.PERMANENT: invalid_lkas_setting_alert,
    ET.NO_ENTRY: NoEntryAlert("LKAS設定無效"),
  },

  EventName.cruiseMismatch: {
    #ET.PERMANENT: ImmediateDisableAlert("openpilot failed to cancel cruise"),
  },

  # openpilot doesn't recognize the car. This switches openpilot into a
  # read-only mode. This can be solved by adding your fingerprint.
  # See https://github.com/commaai/openpilot/wiki/Fingerprinting for more information
  EventName.carUnrecognized: {
    ET.PERMANENT: NormalPermanentAlert("行車記錄模式",
                                       "車輛未識別",
                                       priority=Priority.LOWEST),
  },

  EventName.aeb: {
    ET.PERMANENT: Alert(
      "煞車！",
      "緊急煞車：碰撞風險",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.none, 2.),
    ET.NO_ENTRY: NoEntryAlert("AEB：碰撞風險"),
  },

  EventName.stockAeb: {
    ET.PERMANENT: Alert(
      "煞車！",
      "Stock AEB：碰撞風險",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.none, 2.),
    ET.NO_ENTRY: NoEntryAlert("Stock AEB：碰撞風險"),
  },

  EventName.fcw: {
    ET.PERMANENT: Alert(
      "煞車！",
      "碰撞風險",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.warningSoft, 2.),
  },

  EventName.ldw: {
    ET.PERMANENT: Alert(
      "偵測到車道偏離",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.ldw, AudibleAlert.prompt, 3.),
  },

  # ********** events only containing alerts that display while engaged **********

  EventName.steerTempUnavailableSilent: {
    ET.WARNING: Alert(
      "轉向系統暫時無法使用",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.prompt, 1.8),
  },

  EventName.preDriverDistracted: {
    ET.PERMANENT: Alert(
      "注意",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.promptDriverDistracted: {
    ET.PERMANENT: Alert(
      "注意",
      "駕駛員分心",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.MID, VisualAlert.steerRequired, AudibleAlert.promptDistracted, .1),
  },

  EventName.driverDistracted: {
    ET.PERMANENT: Alert(
      "立即解除控制",
      "駕駛員分心",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.warningImmediate, .1),
  },

  EventName.preDriverUnresponsive: {
    ET.PERMANENT: Alert(
      "觸碰方向盤：未偵測到臉部",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.none, .1),
  },

  EventName.promptDriverUnresponsive: {
    ET.PERMANENT: Alert(
      "觸碰方向盤",
      "駕駛員無反應",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.MID, VisualAlert.steerRequired, AudibleAlert.promptDistracted, .1),
  },

  EventName.driverUnresponsive: {
    ET.PERMANENT: Alert(
      "立即解除",
      "駕駛員無反應",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.warningImmediate, .1),
  },

  EventName.manualRestart: {
    ET.WARNING: Alert(
      "接管控制",
      "手動恢復駕駛",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.resumeRequired: {
    ET.WARNING: Alert(
      "按下恢復鍵以解除待機狀態",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.belowSteerSpeed: {
    ET.WARNING: below_steer_speed_alert,
  },

  EventName.preLaneChangeLeft: {
    ET.WARNING: Alert(
      "向左轉向以在安全時開始變換車道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.preLaneChangeRight: {
    ET.WARNING: Alert(
      "向右轉動方向盤以在安全時開始變換車道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.laneChangeBlocked: {
    ET.WARNING: Alert(
      "盲點偵測到車輛",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.prompt, .1),
  },

  EventName.laneChange: {
    ET.WARNING: Alert(
      "變換車道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.steerSaturated: {
    ET.WARNING: Alert(
      "接管控制",
      "轉向超出系統極限",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.promptRepeat, 2.),
  },

  # Thrown when the fan is driven at >50% but is not rotating
  EventName.fanMalfunction: {
    ET.PERMANENT: NormalPermanentAlert("風扇故障", "可能硬體問題"),
  },

  # Camera is not outputting frames
  EventName.cameraMalfunction: {
    ET.PERMANENT: camera_malfunction_alert,
    ET.SOFT_DISABLE: soft_disable_alert("攝影機故障"),
    ET.NO_ENTRY: NoEntryAlert("攝影機故障：重新啟動您的裝置"),
  },
  # Camera framerate too low
  EventName.cameraFrameRate: {
    ET.PERMANENT: NormalPermanentAlert("攝影機幀率過低", "重新啟動您的裝置"),
    ET.SOFT_DISABLE: soft_disable_alert("攝影機幀率過低"),
    ET.NO_ENTRY: NoEntryAlert("攝影機幀率過低：請重新啟動裝置"),
  },

  # Unused

  EventName.locationdTemporaryError: {
    ET.NO_ENTRY: NoEntryAlert("locationd 暫時性錯誤"),
    ET.SOFT_DISABLE: soft_disable_alert("locationd 暫時性錯誤"),
  },

  EventName.locationdPermanentError: {
    ET.NO_ENTRY: NoEntryAlert("locationd 永久性錯誤"),
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("locationd 永久性錯誤"),
    ET.PERMANENT: NormalPermanentAlert("locationd 永久性錯誤"),
  },

  # openpilot tries to learn certain parameters about your car by observing
  # how the car behaves to steering inputs from both human and openpilot driving.
  # This includes:
  # - steer ratio: gear ratio of the steering rack. Steering angle divided by tire angle
  # - tire stiffness: how much grip your tires have
  # - angle offset: most steering angle sensors are offset and measure a non zero angle when driving straight
  # This alert is thrown when any of these values exceed a sanity check. This can be caused by
  # bad alignment or bad sensor data. If this happens consistently consider creating an issue on GitHub
  EventName.paramsdTemporaryError: {
    ET.NO_ENTRY: paramsd_invalid_alert,
    ET.SOFT_DISABLE: soft_disable_alert("參數暫時性錯誤"),
  },

  EventName.paramsdPermanentError: {
    ET.NO_ENTRY: NoEntryAlert("參數永久性錯誤"),
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("參數永久性錯誤"),
    ET.PERMANENT: NormalPermanentAlert("參數永久性錯誤"),
  },

  # ********** events that affect controls state transitions **********

  EventName.pcmEnable: {
    ET.ENABLE: EngagementAlert(AudibleAlert.engage),
  },

  EventName.buttonEnable: {
    ET.ENABLE: EngagementAlert(AudibleAlert.engage),
  },

  EventName.pcmDisable: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
  },

  EventName.buttonCancel: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("已取消按下"),
  },

  EventName.brakeHold: {
    ET.WARNING: Alert(
      "按下恢復鍵解除煞車保持",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.parkBrake: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("駐車煞車已啟用"),
  },

  EventName.pedalPressed: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("踏板已踩下",
                              visual_alert=VisualAlert.brakePressed),
  },

  EventName.steerDisengage: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("方向盤已介入"),
  },

  EventName.preEnableStandstill: {
    ET.PRE_ENABLE: Alert(
      "釋放煞車以啟用",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1, creation_delay=1.),
  },

  EventName.gasPressedOverride: {
    ET.OVERRIDE_LONGITUDINAL: Alert(
      "",
      "",
      AlertStatus.normal, AlertSize.none,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.steerOverride: {
    ET.OVERRIDE_LATERAL: Alert(
      "",
      "",
      AlertStatus.normal, AlertSize.none,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.wrongCarMode: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: wrong_car_mode_alert,
  },

  EventName.resumeBlocked: {
    ET.NO_ENTRY: NoEntryAlert("按下設定以啟用"),
  },

  EventName.wrongCruiseMode: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("自適應巡航已停用"),
  },

  EventName.steerTempUnavailable: {
    ET.SOFT_DISABLE: soft_disable_alert("轉向系統暫時無法使用"),
    ET.NO_ENTRY: NoEntryAlert("轉向系統暫時無法使用"),
  },

  EventName.steerTimeLimit: {
    ET.SOFT_DISABLE: soft_disable_alert("車輛轉向時間限制"),
    ET.NO_ENTRY: NoEntryAlert("車輛轉向時間限制"),
  },

  EventName.outOfSpace: {
    ET.PERMANENT: out_of_space_alert,
    ET.NO_ENTRY: NoEntryAlert("儲存空間不足"),
  },

  EventName.belowEngageSpeed: {
    ET.NO_ENTRY: below_engage_speed_alert,
  },

  EventName.sensorDataInvalid: {
    ET.PERMANENT: Alert(
      "感測器數據無效",
      "可能硬體問題",
      AlertStatus.normal, AlertSize.mid,
      Priority.LOWER, VisualAlert.none, AudibleAlert.none, .2, creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("感測器數據無效"),
    ET.SOFT_DISABLE: soft_disable_alert("感測器數據無效"),
  },

  EventName.noGps: {
  },

  EventName.tooDistracted: {
    ET.NO_ENTRY: NoEntryAlert("注意力分散程度過高"),
  },

  EventName.excessiveActuation: {
    ET.SOFT_DISABLE: soft_disable_alert("過度作動"),
    ET.NO_ENTRY: NoEntryAlert("過度作動"),
  },

  EventName.overheat: {
    ET.PERMANENT: overheat_alert,
    ET.SOFT_DISABLE: soft_disable_alert("系統過熱"),
    ET.NO_ENTRY: NoEntryAlert("系統過熱"),
  },

  EventName.wrongGear: {
    ET.SOFT_DISABLE: user_soft_disable_alert("檔位非 D 檔"),
    ET.NO_ENTRY: NoEntryAlert("檔位非 D 檔"),
  },

  # This alert is thrown when the calibration angles are outside of the acceptable range.
  # For example if the device is pointed too much to the left or the right.
  # Usually this can only be solved by removing the mount from the windshield completely,
  # and attaching while making sure the device is pointed straight forward and is level.
  # See https://comma.ai/setup for more information
  EventName.calibrationInvalid: {
    ET.PERMANENT: calibration_invalid_alert,
    ET.SOFT_DISABLE: soft_disable_alert("校準無效：請重新安裝裝置並再次校準"),
    ET.NO_ENTRY: NoEntryAlert("校準無效：請重新安裝裝置並再次校準"),
  },

  EventName.calibrationIncomplete: {
    ET.PERMANENT: calibration_incomplete_alert,
    ET.SOFT_DISABLE: soft_disable_alert("校準未完成"),
    ET.NO_ENTRY: NoEntryAlert("校準中"),
  },

  EventName.calibrationRecalibrating: {
    ET.PERMANENT: calibration_incomplete_alert,
    ET.SOFT_DISABLE: soft_disable_alert("偵測到裝置重新安裝：重新校準中"),
    ET.NO_ENTRY: NoEntryAlert("偵測到重新安裝：重新校準中"),
  },

  EventName.doorOpen: {
    ET.SOFT_DISABLE: user_soft_disable_alert("車門開啟"),
    ET.NO_ENTRY: NoEntryAlert("車門開啟"),
  },

  EventName.seatbeltNotLatched: {
    ET.SOFT_DISABLE: user_soft_disable_alert("安全帶未扣"),
    ET.NO_ENTRY: NoEntryAlert("安全帶未扣上"),
  },

  EventName.espDisabled: {
    ET.SOFT_DISABLE: soft_disable_alert("電子穩定控制系統已停用"),
    ET.NO_ENTRY: NoEntryAlert("電子穩定控制系統已停用"),
  },

  EventName.lowBattery: {
    ET.SOFT_DISABLE: soft_disable_alert("電量不足"),
    ET.NO_ENTRY: NoEntryAlert("電量不足"),
  },

  # Different openpilot services communicate between each other at a certain
  # interval. If communication does not follow the regular schedule this alert
  # is thrown. This can mean a service crashed, did not broadcast a message for
  # ten times the regular interval, or the average interval is more than 10% too high.
  EventName.commIssue: {
    ET.SOFT_DISABLE: soft_disable_alert("進程間通訊異常"),
    ET.NO_ENTRY: comm_issue_alert,
  },
  EventName.commIssueAvgFreq: {
    ET.SOFT_DISABLE: soft_disable_alert("進程間通訊率過低"),
    ET.NO_ENTRY: NoEntryAlert("進程間通訊率過低"),
  },

  EventName.selfdrivedLagging: {
    ET.SOFT_DISABLE: soft_disable_alert("系統延遲"),
    ET.NO_ENTRY: NoEntryAlert("自駕程序延遲：請重啟您的裝置"),
  },

  # Thrown when manager detects a service exited unexpectedly while driving
  EventName.processNotRunning: {
    ET.NO_ENTRY: process_not_running_alert,
    ET.SOFT_DISABLE: soft_disable_alert("程序未執行"),
  },

  EventName.radarFault: {
    ET.SOFT_DISABLE: soft_disable_alert("雷達錯誤：請重新啟動車輛"),
    ET.NO_ENTRY: NoEntryAlert("雷達錯誤：請重新啟動車輛"),
  },

  EventName.radarTempUnavailable: {
    ET.SOFT_DISABLE: soft_disable_alert("雷達暫時無法使用"),
    ET.NO_ENTRY: NoEntryAlert("雷達暫時無法使用"),
  },

  # Every frame from the camera should be processed by the model. If modeld
  # is not processing frames fast enough they have to be dropped. This alert is
  # thrown when over 20% of frames are dropped.
  EventName.modeldLagging: {
    ET.SOFT_DISABLE: soft_disable_alert("駕駛模式延遲"),
    ET.NO_ENTRY: NoEntryAlert("駕駛模式延遲"),
    ET.PERMANENT: modeld_lagging_alert,
  },

  # Besides predicting the path, lane lines and lead car data the model also
  # predicts the current velocity and rotation speed of the car. If the model is
  # very uncertain about the current velocity while the car is moving, this
  # usually means the model has trouble understanding the scene. This is used
  # as a heuristic to warn the driver.
  EventName.posenetInvalid: {
    ET.SOFT_DISABLE: soft_disable_alert("Posenet 速度無效"),
    ET.NO_ENTRY: posenet_invalid_alert,
  },

  # When the localizer detects an acceleration of more than 40 m/s^2 (~4G) we
  # alert the driver the device might have fallen from the windshield.
  EventName.deviceFalling: {
    ET.SOFT_DISABLE: soft_disable_alert("裝置已從支架脫落"),
    ET.NO_ENTRY: NoEntryAlert("裝置從支架脫落"),
  },

  EventName.lowMemory: {
    ET.SOFT_DISABLE: soft_disable_alert("記憶體不足：請重新啟動裝置"),
    ET.PERMANENT: low_memory_alert,
    ET.NO_ENTRY: NoEntryAlert("記憶體不足：請重新啟動裝置"),
  },

  EventName.accFaulted: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("巡航系統故障：請重新啟動車輛"),
    ET.PERMANENT: NormalPermanentAlert("巡航系統故障：請重啟車輛以啟用"),
    ET.NO_ENTRY: NoEntryAlert("巡航系統故障：請重新啟動車輛"),
  },

  EventName.espActive: {
    ET.SOFT_DISABLE: soft_disable_alert("電子穩定控制系統作動中"),
    ET.NO_ENTRY: NoEntryAlert("電子穩定控制系統啟動中"),
  },

  EventName.controlsMismatch: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("控制系統不符"),
    ET.NO_ENTRY: NoEntryAlert("控制系統不符"),
  },

  # Sometimes the USB stack on the device can get into a bad state
  # causing the connection to the panda to be lost
  EventName.usbError: {
    ET.SOFT_DISABLE: soft_disable_alert("USB錯誤：請重啟您的裝置"),
    ET.PERMANENT: NormalPermanentAlert("USB錯誤：請重啟您的裝置"),
    ET.NO_ENTRY: NoEntryAlert("USB錯誤：請重啟您的裝置"),
  },

  # This alert can be thrown for the following reasons:
  # - No CAN data received at all
  # - CAN data is received, but some message are not received at the right frequency
  # If you're not writing a new car port, this is usually cause by faulty wiring
  EventName.canError: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("CAN匯流排錯誤"),
    ET.PERMANENT: Alert(
      "CAN 錯誤：檢查連線",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, 1., creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("CAN 錯誤：檢查連線"),
  },

  EventName.canBusMissing: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("CAN 匯流排已斷開"),
    ET.PERMANENT: Alert(
      "CAN匯流排連線中斷：可能為線路故障",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, 1., creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("CAN 匯流排已斷開：請檢查連接"),
  },

  EventName.steerUnavailable: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("LKAS故障：請重新啟動車輛"),
    ET.PERMANENT: NormalPermanentAlert("LKAS故障：請重新啟動車輛以啟用系統"),
    ET.NO_ENTRY: NoEntryAlert("LKAS故障：請重新啟動車輛"),
  },

  EventName.reverseGear: {
    ET.PERMANENT: Alert(
      "倒車檔",
      "",
      AlertStatus.normal, AlertSize.full,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .2, creation_delay=0.5),
    ET.USER_DISABLE: ImmediateDisableAlert("倒車檔"),
    ET.NO_ENTRY: NoEntryAlert("倒車檔"),
  },

  # On cars that use stock ACC the car can decide to cancel ACC for various reasons.
  # When this happens we can no long control the car so the user needs to be warned immediately.
  EventName.cruiseDisabled: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("巡航系統已關閉"),
  },

  # When the relay in the harness box opens the CAN bus between the LKAS camera
  # and the rest of the car is separated. When messages from the LKAS camera
  # are received on the car side this usually means the relay hasn't opened correctly
  # and this alert is thrown.
  EventName.relayMalfunction: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("線束繼電器故障"),
    ET.PERMANENT: NormalPermanentAlert("線束繼電器故障", "檢查硬體"),
    ET.NO_ENTRY: NoEntryAlert("線束繼電器故障"),
  },

  EventName.speedTooLow: {
    ET.IMMEDIATE_DISABLE: Alert(
      "openpilot 已取消",
      "車速過低",
      AlertStatus.normal, AlertSize.mid,
      Priority.HIGH, VisualAlert.none, AudibleAlert.disengage, 3.),
  },

  # When the car is driving faster than most cars in the training data, the model outputs can be unpredictable.
  EventName.speedTooHigh: {
    ET.WARNING: Alert(
      "車速過高",
      "在此速度下模式不確定",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.promptRepeat, 4.),
    ET.NO_ENTRY: NoEntryAlert("減速以啟用"),
  },

  EventName.vehicleSensorsInvalid: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("車輛感測器失效"),
    ET.PERMANENT: NormalPermanentAlert("車輛傳感器校準中", "行駛以進行校準"),
    ET.NO_ENTRY: NoEntryAlert("車輛傳感器校準中"),
  },

  EventName.personalityChanged: {
    ET.WARNING: personality_changed_alert,
  },

  EventName.userBookmark: {
    ET.PERMANENT: NormalPermanentAlert("書籤已儲存", duration=1.5),
  },

  EventName.audioFeedback: {
    ET.PERMANENT: audio_feedback_alert,
  },
}


if __name__ == '__main__':
  # print all alerts by type and priority
  from cereal.services import SERVICE_LIST
  from collections import defaultdict

  event_names = {v: k for k, v in EventName.schema.enumerants.items()}
  alerts_by_type: dict[str, dict[Priority, list[str]]] = defaultdict(lambda: defaultdict(list))

  CP = car.CarParams.new_message()
  CS = car.CarState.new_message()
  sm = messaging.SubMaster(list(SERVICE_LIST.keys()))

  for i, alerts in EVENTS.items():
    for et, alert in alerts.items():
      if callable(alert):
        alert = alert(CP, CS, sm, False, 1, log.LongitudinalPersonality.standard)
      alerts_by_type[et][alert.priority].append(event_names[i])

  all_alerts: dict[str, list[tuple[Priority, list[str]]]] = {}
  for et, priority_alerts in alerts_by_type.items():
    all_alerts[et] = sorted(priority_alerts.items(), key=lambda x: x[0], reverse=True)

  for status, evs in sorted(all_alerts.items(), key=lambda x: x[0]):
    print(f"**** {status} ****")
    for p, alert_list in evs:
      print(f"  {repr(p)}:")
      print("   ", ', '.join(alert_list), "\n")