#!/usr/bin/env python3
"""Maintain onroad/offroad alert and UI strings in app_zh-CHS.po (ADAS terminology)."""

import os
import re
from pathlib import Path

BASEDIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
PO_PATH = os.path.join(BASEDIR, "selfdrive/ui/translations/app_zh-CHS.po")

# ADAS industry terminology (Simplified Chinese)
ALERT_TRANSLATIONS = {
  "openpilot will disengage": "辅助驾驶即将退出",
  "Always keep hands on wheel and eyes on road": "请始终手扶方向盘、注视前方道路",
  "BRAKE!": "请制动！",
  "Risk of Collision": "前向碰撞风险",
  "Emergency Braking: Risk of Collision": "自动紧急制动：前向碰撞风险",
  "AEB: Risk of Collision": "AEB：前向碰撞风险",
  "Stock AEB: Risk of Collision": "原厂 AEB：前向碰撞风险",
  "Stock LKAS: Lane Departure Detected": "原厂 LKAS：车道偏离",
  "Lane Departure Detected": "车道偏离",
  "Pay Attention": "请注意",
  "Driver Distracted": "驾驶员分心",
  "DISENGAGE IMMEDIATELY": "请立即退出辅助驾驶",
  "Touch Steering Wheel: No Face Detected": "请手扶方向盘：未检测到人脸",
  "Touch Steering Wheel": "请手扶方向盘",
  "Driver Unresponsive": "驾驶员无响应",
  "Distraction Level Too High": "分心程度过高",
  "TAKE CONTROL": "请立即接管",
  "Take Control": "请立即接管",
  "take control": "请立即接管",
  "TAKE CONTROL IMMEDIATELY": "请立即接管",
  "Resume Driving Manually": "请手动继续驾驶",
  "Press Resume to Exit Standstill": "按 RES 键退出停走模式",
  "Press Resume to Exit Brake Hold": "按 RES 键退出制动保持",
  "Press Resume": "按 RES",
  "Press Set to Engage": "按 SET 键激活辅助驾驶",
  "Release Brake to Engage": "松开制动以激活辅助驾驶",
  "Cancel Pressed": "已按下取消键",
  "Steer Left to Start Lane Change Once Safe": "确认安全后向左拨杆开始变道",
  "Steer Right to Start Lane Change Once Safe": "确认安全后向右拨杆开始变道",
  "Car Detected in Blindspot": "盲区监测到车辆",
  "Car in Blindspot": "盲区有车辆",
  "Changing Lanes": "正在变道",
  "Confirm Lane Change": "确认变道",
  "Steer Left": "向左变道",
  "Steer Right": "向右变道",
  "Turn Exceeds Steering Limit": "转向超出限制",
  "turn exceeds limit": "转向超出限制",
  "Slow down to engage": "请减速后激活辅助驾驶",
  "Speed Too High": "车速过高",
  "Speed too low": "车速过低",
  "Model uncertain at this speed": "当前车速下模型不确定",
  "Door Open": "车门未关",
  "Seatbelt Unlatched": "安全带未系",
  "Parking Brake Engaged": "电子驻车已启用",
  "Pedal Pressed": "踏板已踩下",
  "Steering Pressed": "方向盘已介入",
  "Gear not D": "未挂 D 挡",
  "Reverse Gear": "倒挡",
  "Reverse": "倒车",
  "Reverse\nGear": "倒\n挡",
  "Adaptive Cruise Disabled": "ACC 已禁用",
  "Cruise Is Off": "巡航已关闭",
  "Cruise Fault: Restart the Car": "巡航故障：请重启车辆",
  "Cruise Fault: Restart the car to engage": "巡航故障：重启车辆以激活辅助驾驶",
  "LKAS Fault: Restart the Car": "LKAS 故障：请重启车辆",
  "LKAS Fault: Restart the car to engage": "LKAS 故障：重启车辆以激活辅助驾驶",
  "Invalid LKAS setting": "LKAS 设置无效",
  "Toggle stock LKAS on or off to engage": "切换原厂 LKAS 以激活辅助驾驶",
  "Switch to Traffic-Aware Cruise Control to engage": "切换至 TACC 以激活辅助驾驶",
  "Enable your car's LKAS to engage": "启用车辆 LKAS 以激活辅助驾驶",
  "Disable your car's stock LKAS to engage": "禁用原厂 LKAS 以激活辅助驾驶",
  "Enable Adaptive Cruise to Engage": "启用 ACC 以激活辅助驾驶",
  "Enable Main Switch to Engage": "启用主开关以激活辅助驾驶",
  "Calibrating": "标定中",
  "Recalibrating": "重新标定中",
  "Calibration Incomplete": "标定未完成",
  "Calibration in Progress": "标定进行中",
  "Calibration Invalid": "标定无效",
  "Calibration Invalid: Remount Device & Recalibrate": "标定无效：请重新安装设备并标定",
  "Drive to Calibrate": "请行驶以完成标定",
  "Device Remount Detected: Recalibrating": "检测到设备重新安装：重新标定中",
  "Remount Detected: Recalibrating": "检测到重新安装：重新标定中",
  "Device Fell Off Mount": "设备脱离安装支架",
  "Vehicle Sensors Calibrating": "车辆传感器标定中",
  "Vehicle Sensors Invalid": "车辆传感器无效",
  "Steering misalignment detected": "检测到转向不对中",
  "Steer ratio mismatch": "转向比不匹配",
  "Abnormal tire stiffness": "轮胎刚度异常",
  "Camera Malfunction": "摄像头故障",
  "Camera Malfunction: Reboot Your Device": "摄像头故障：请重启设备",
  "Camera Frame Rate Low": "摄像头帧率过低",
  "Camera Frame Rate Low: Reboot Your Device": "摄像头帧率过低：请重启设备",
  "Reboot your Device": "请重启设备",
  "Fan Malfunction": "风扇故障",
  "Likely Hardware Issue": "可能存在硬件问题",
  "Possible Hardware Issue": "可能存在硬件问题",
  "Check Hardware": "请检查硬件",
  "Harness Relay Malfunction": "线束继电器故障",
  "USB Error: Reboot Your Device": "USB 错误：请重启设备",
  "Low Battery": "电量低",
  "System Initializing": "系统初始化中",
  "System Overheated": "系统过热",
  "System Lagging": "系统延迟",
  "Low Memory": "内存不足",
  "Low Memory: Reboot Your Device": "内存不足：请重启设备",
  "High CPU Usage": "CPU 使用率过高",
  "Selfdrive Process Lagging: Reboot Your Device": "辅助驾驶进程延迟：请重启设备",
  "Driving Model Lagging": "规划模型延迟",
  "Process Not Running": "进程未运行",
  "Communication Issue Between Processes": "进程间通信异常",
  "Low Communication Rate Between Processes": "进程间通信速率过低",
  "CAN Bus Disconnected": "CAN 总线断开",
  "CAN Bus Disconnected: Check Connections": "CAN 总线断开：请检查连接",
  "CAN Bus Disconnected: Likely Faulty Cable": "CAN 总线断开：线缆可能故障",
  "locationd Temporary Error": "locationd 临时错误",
  "locationd Permanent Error": "locationd 永久错误",
  "paramsd Temporary Error": "paramsd 临时错误",
  "paramsd Permanent Error": "paramsd 永久错误",
  "Posenet Speed Invalid": "Posenet 速度无效",
  "Sensor Data Invalid": "传感器数据无效",
  "Controls Mismatch": "控制不匹配",
  "Excessive Actuation": "执行器动作过度",
  "Vehicle Steering Time Limit": "车辆转向时间限制",
  "Steering Assist Temporarily Unavailable": "转向辅助暂时不可用",
  "Steering Temporarily Unavailable": "转向暂时不可用",
  "Electronic Stability Control Active": "ESC 已激活",
  "Electronic Stability Control Disabled": "ESC 已禁用",
  "Dashcam Mode": "行车记录仪模式",
  "Dashcam mode": "行车记录仪模式",
  "Dashcam mode for unsupported car": "不支持车型的行车记录仪模式",
  "Car Unrecognized": "车辆未识别",
  "Security Key Not Available": "安全密钥不可用",
  "Be ready to take over at any time": "请随时准备接管",
  "WARNING: This branch is not tested": "警告：此分支未经测试",
  "Joystick Mode": "摇杆模式",
  "Ensure road ahead is clear": "请确认前方道路畅通",
  "Bookmark Saved": "书签已保存",
  "Recording Audio Feedback": "正在录制音频反馈",
  "Out of Storage": "存储空间不足",
  "Radar Error: Restart the Car": "雷达错误：请重启车辆",
  "Radar Temporarily Unavailable": "雷达暂时不可用",
  "Unknown Vehicle Variant": "未知车型变体",
  "openpilot Canceled": "openpilot 已取消",
  "openpilot failed to cancel cruise": "openpilot 取消巡航失败",
  "Drive above {} to engage": "提速至 {} 以上以激活辅助驾驶",
  "Steer Assist Unavailable Below {}": "低于 {} 时转向辅助不可用",
  "Drive Above {}": "提速至 {} 以上",
  "{} second remaining. Press again to save early.": "剩余 {} 秒。再次按下可提前保存。",
  "{} seconds remaining. Press again to save early.": "剩余 {} 秒。再次按下可提前保存。",
  "{}% full": "已使用 {}%",
  "Speed Error: {} m/s": "速度误差：{} m/s",
  "Remount Device (Pitch: {}°, Yaw: {}°)": "重新安装设备（俯仰：{}°，偏航：{}°）",
  "Angle offset too high (Offset: {}°)": "角度偏移过大（偏移：{}°）",
  "Steering rack geometry may be off (Ratio: {})": "转向齿条几何可能异常（比率：{}）",
  "Check tires, pressure, or alignment (Factor: {})": "请检查轮胎、胎压或四轮定位（系数：{}）",
  "{} °C": "{} °C",
  "{}% used": "已使用 {}%",
  "{}% frames dropped": "丢帧 {}%",
  "Gas: {}%, Steer: {}%": "油门：{}%，转向：{}%",
  "Driving Personality: {}": "驾驶风格：{}",
  "Calibrating: {}%": "标定中：{}%",
  "Recalibrating: {}%": "重新标定中：{}%",
  "Device temperature too high. System cooling down before starting. Current internal component temperature: %1":
    "设备温度过高，正在降温，完成后方可进入行驶模式。当前内部组件温度：%1",
  "Immediately connect to the internet to check for updates. If you do not connect to the internet, openpilot won't engage in %1":
    "请立即连接互联网检查更新。若不连接，%1 后将无法激活辅助驾驶",
  "Connect to internet to check for updates. openpilot won't automatically start until it connects to internet to check for updates.":
    "请连接互联网检查更新。连接前无法自动进入行驶模式。",
  "Unable to download updates\n%1": "无法下载更新\n%1",
  "Taking camera snapshots. System won't start until finished.":
    "正在拍摄摄像头快照，完成后方可进入行驶模式。",
  "An update to your device's operating system is downloading in the background. You will be prompted to update when it's ready to install.":
    "设备操作系统更新正在后台下载。准备就绪时将提示您安装。",
  "Failed to register with comma.ai backend. It will not connect or upload to comma.ai servers, and receives no support from comma.ai. If this is a device purchased at comma.ai/shop, open a ticket at https://comma.ai/support.":
    "无法向 comma.ai 后端注册。设备不会连接或上传至 comma.ai 服务器，也不获得 comma.ai 支持。若设备购自 comma.ai/shop，请至 https://comma.ai/support 提交工单。",
  "openpilot was unable to identify your car. Your car is either unsupported or its ECUs are not recognized. Please submit a pull request to add the firmware versions to the proper vehicle. Need help? Join discord.comma.ai.":
    "openpilot 无法识别您的车辆。车辆可能不受支持，或 ECU 未被识别。请提交 PR 将固件版本添加到对应车型。需要帮助？加入 discord.comma.ai。",
  "openpilot detected a change in the device's mounting position. Ensure the device is fully seated in the mount and the mount is firmly secured to the windshield.":
    "openpilot 检测到设备安装位置发生变化。请确保设备完全装入支架，且支架牢固固定在前挡风玻璃上。",
  "Poor visibility detected for driver monitoring. Ensure the device has a clear view of the driver. This can be checked in the device settings. Extreme lighting conditions and/or unconventional mounting positions may also trigger this alert.":
    "驾驶员监测视野不佳。请确保设备能清晰看到驾驶员，可在设备设置中检查。极端光照或不常规安装位置也可能触发此告警。",
  "Excessive %1 actuation detected on your last drive. Please contact support at https://comma.ai/support and share your device's Dongle ID for troubleshooting.":
    "检测到上次驾驶中 %1 执行器动作过度。请联系 https://comma.ai/support 并提供设备 Dongle ID 以便排查。",
}

UI_TRANSLATIONS = {
  "On/Off Road": "行驶模式/驻车模式",
  "Go Offroad": "进入驻车模式",
  "Go Onroad": "进入行驶模式",
  "Force openpilot to go into onroad/offroad state.<br>(e.g. for update purpose)":
    "强制切换行驶/驻车模式。<br>（例如用于系统更新）",
  "Disengage on Accelerator Pedal": "加速踏板退出辅助驾驶",
  "Disengage to Power Off": "退出辅助驾驶后关机",
  "Disengage to Reboot": "退出辅助驾驶后重启",
  "Disengage to Reset Calibration": "退出辅助驾驶后重置标定",
  "When enabled, pressing the accelerator pedal will disengage openpilot.":
    "启用后，踩下加速踏板将退出辅助驾驶。",
  "Reset Calibration": "重置标定",
  "Are you sure you want to reset calibration?": "确定要重置标定吗？",
  "openpilot is continuously calibrating, resetting is rarely required. Resetting calibration will restart openpilot if the car is powered on.":
    "openpilot 会持续标定，通常无需重置。若车辆已通电，重置标定将重启 openpilot。",
  "Always-On Driver Monitoring": "始终开启驾驶员监测",
  "Enable driver monitoring even when openpilot is not engaged.":
    "未激活辅助驾驶时也启用驾驶员监测。",
  "Enable Lane Departure Warnings": "启用车道偏离预警",
  "Upload data from the driver facing camera and help improve the driver monitoring algorithm.":
    "上传车内摄像头数据，帮助改进驾驶员监测算法。",
  "Updates are only downloaded while the car is off.":
    "仅在驻车模式下下载更新。",
  " Steering torque response calibration is complete.": " 转向扭矩响应标定完成。",
  " Steering torque response calibration is {}% complete.": " 转向扭矩响应标定已完成 {}%。",
  "<br><br>Steering lag calibration is complete.": "<br><br>转向延迟标定完成。",
  "<br><br>Steering lag calibration is {}% complete.": "<br><br>转向延迟标定已完成 {}%。",
  "Waiting to start": "等待进入行驶模式",
}


def _po_quote(s: str) -> str:
  return '"' + s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n') + '"'


def _source_ref(msgid: str) -> str:
  if "%1" in msgid:
    return "openpilot/selfdrive/selfdrived/alerts_offroad.json"
  if "{}" in msgid:
    return "openpilot/selfdrive/ui/lib/tr_alert.py"
  return "openpilot/selfdrive/selfdrived/events.py"


def _upsert_entry(content: str, msgid: str, msgstr: str) -> tuple[str, bool, bool]:
  quoted_id = _po_quote(msgid)
  quoted_str = _po_quote(msgstr)
  block_re = rf'(msgid {re.escape(quoted_id)}\nmsgstr )("(?:\\.|[^"\\])*")'
  match = re.search(block_re, content)
    if match:
      if match.group(2) == quoted_str:
        return content, False, False
      content = re.sub(block_re, lambda m: m.group(1) + quoted_str, content, count=1)
      return content, False, True

  append = f"#: {_source_ref(msgid)}\nmsgid {quoted_id}\nmsgstr {quoted_str}"
  return content.rstrip() + "\n\n" + append + "\n", True, False


def _validate_po(path: str) -> None:
  for i, line in enumerate(Path(path).read_text(encoding='utf-8').splitlines(), 1):
    if line.startswith('msgstr "') and not line.endswith('"'):
      raise ValueError(f"invalid msgstr at line {i}: {line!r}")


def merge():
  with open(PO_PATH, encoding='utf-8') as f:
    content = f.read()

  added = updated = 0
  for translations in (ALERT_TRANSLATIONS, UI_TRANSLATIONS):
    for msgid, msgstr in translations.items():
      content, was_added, was_updated = _upsert_entry(content, msgid, msgstr)
      added += int(was_added)
      updated += int(was_updated)

  with open(PO_PATH, 'w', encoding='utf-8') as f:
    f.write(content)

  # Verify the written file parses cleanly
  _validate_po(PO_PATH)

  print(f"added {added}, updated {updated} entries in {PO_PATH}")


if __name__ == "__main__":
  merge()
