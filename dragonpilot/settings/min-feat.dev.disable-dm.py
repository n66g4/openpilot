from dragonpilot.settings import tr

ITEMS = [
  {
    "section": "Device",
    "key": "dp_dev_disable_dm",
    "type": "toggle_item",
    "title": lambda: tr("Disable Driver Monitoring"),
    "description": lambda: tr("Stop driver monitoring and its distraction alerts, same as on Lite hardware. Takes effect on next drive."),
    "condition": "not LITE",
    "flags": "PERSISTENT",
    "param_type": "BOOL",
    "default": "0",
  },
]
