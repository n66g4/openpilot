from dragonpilot.settings import tr

ITEMS = [
  {
    "section": "Device",
    "key": "dp_dev_disable_dm",
    "type": "toggle_item",
    "title": lambda: tr("Disable Driver Monitoring"),
    "description": lambda: tr("Completely disable driver monitoring. Changing this setting will restart openpilot if the car is powered on."),
    "condition": "not LITE",
    "flags": "PERSISTENT",
    "param_type": "BOOL",
    "default": "0",
    "callback": "_on_disable_dm_toggle",
  },
  {
    "section": "Device",
    "key": "dp_dev_is_rhd",
    "type": "toggle_item",
    "title": lambda: tr("Enable Right-Hand Drive Mode"),
    "description": lambda: tr("Allow openpilot to obey right-hand traffic conventions on right driver seat."),
    "condition": "LITE or dp_dev_disable_dm",
    "flags": "PERSISTENT",
    "param_type": "BOOL",
    "default": "0",
  },
  {
    "section": "Device",
    "key": "dp_dev_beep",
    "type": "toggle_item",
    "title": lambda: tr("Enable Beep (Warning)"),
    "description": lambda: tr("Use Buzzer for audiable alerts."),
    "condition": "LITE",
    "flags": "PERSISTENT",
    "param_type": "BOOL",
    "default": "0",
  },
]
