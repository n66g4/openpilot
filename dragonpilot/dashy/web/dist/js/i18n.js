/**
 * Dashy web UI i18n. Chrome strings only; settings title/description come from server.
 */
(function () {
  "use strict";

  const ZH = {
    "Controls": "控制",
    "Settings": "设置",
    "Files": "文件",
    "DEVICE CONTROLS": "设备控制",
    "FILE BROWSER": "文件浏览",
    "VEHICLE": "车辆",
    "Vehicle": "车辆",
    "No vehicle models available. Set dp_dev_model_list on the device to populate this list.": "暂无可用车型。请在设备上设置 dp_dev_model_list 以填充列表。",
    "Force Offroad": "强制驻车模式",
    "Force Offroad?": "强制进入驻车模式？",
    "Temporarily go offroad to update system.": "临时进入驻车模式以便更新系统。",
    "This will temporarily disable driving mode. You can update the system while offroad.": "将暂时退出行驶模式，可在驻车模式下更新系统。",
    "Dashy Visual": "Dashy 视觉",
    "Connecting to device...": "正在连接设备...",
    "Vehicle Model": "车型",
    "Select your vehicle or use AUTO for detection.": "选择车型，或使用 AUTO 自动识别。",
    "Select Vehicle Model": "选择车型",
    "Reboot Device": "重启设备",
    "Reboot Device?": "重启设备？",
    "This will reboot the device to apply changes.": "将重启设备以应用更改。",
    "Reboot": "重启",
    "Rainbow Road": "彩虹路径",
    "Animated rainbow gradient on the driving path. Scrolls with vehicle speed.": "在行驶路径上显示彩虹渐变动画，随车速滚动。",
    "Lead Stats Panel": "前车统计面板",
    "Show distance, speed, and TTC for the lead vehicle at the bottom of the HUD.": "在 HUD 底部显示前车距离、速度与碰撞时间 (TTC)。",
    "Heads-up Display (HUD) Mode": "抬头显示 (HUD) 模式",
    "Mirror the display for windshield projection.": "镜像显示以便投射到挡风玻璃。",
    "[AUTO]": "[自动检测]",
    "Search...": "搜索...",
    "No results found": "未找到结果",
    "Cancel": "取消",
    "Confirm": "确认",
    "Save": "保存",
    "Saving…": "保存中…",
    "Saved": "已保存",
    "Running…": "运行中…",
    "Done": "完成",
    "Error": "错误",
    "Run": "运行",
    "(empty)": "(空)",
    "Name": "名称",
    "Last Modified": "修改时间",
    "Size": "大小",
    "root": "根目录",
    ".. (Parent Directory)": ".. (上级目录)",
    "Failed to load files:": "加载文件失败：",
    "Server unavailable. Retrying...": "服务器不可用，正在重试...",
    "Connecting...": "连接中...",
    "Connected": "已连接",
    "Disconnected. Reconnecting...": "已断开，正在重连...",
    "Error. Reconnecting...": "出错，正在重连...",
    "Data stalled. Reconnecting...": "数据停滞，正在重连...",
    "No data from dashyd": "未收到 dashyd 数据",
    "Data stale": "数据过期",
    "Off": "关",
    "On": "开",
    "Dashy by dragonpilot": "Dashy by dragonpilot",
    "openpilot Unavailable": "openpilot 不可用",
    "Waiting to start": "等待进入行驶模式",
    "TAKE CONTROL IMMEDIATELY": "请立即接管",
    "System Unresponsive": "系统无响应",
    "DISENGAGED": "已退出",
  };

  const TABLES = {
    "zh-CHS": ZH,
    "zh-CHT": ZH,
  };

  let lang = "en";
  let table = null;

  function setLang(code) {
    lang = String(code || "en").replace(/^main_/, "");
    table = TABLES[lang] || null;
    const status = document.getElementById("loading-status");
    if (status && table && status.textContent === "Connecting to device...") {
      status.textContent = table["Connecting to device..."] || status.textContent;
    }
    if (table) {
      document.documentElement.lang = lang.startsWith("zh") ? "zh" : "en";
      if (document.title === "Dashy by dragonpilot") {
        document.title = table["Dashy by dragonpilot"] || document.title;
      }
    }
  }

  function t(text) {
    if (!text || !table) return text;
    return table[text] || text;
  }

  window.__dashySetLang = setLang;
  window.__dashyT = t;
  window.__dashyLang = () => lang;
})();
