from dragonpilot.settings import tr

ITEMS = [
  {
    "section": "Device",
    "key": "dp_athena_host",
    "type": "text_input_item",
    "title": lambda: tr("Athena Host"),
    "description": lambda: tr("WebSocket URL for self-hosted openpilot-server."),
    "flags": "PERSISTENT",
    "param_type": "STRING",
    "default": "ws://lirou.fun:7899",
    "reboot_on_change": True,
  },
  {
    "section": "Device",
    "key": "dp_api_host",
    "type": "text_input_item",
    "title": lambda: tr("API Host"),
    "description": lambda: tr("HTTP URL for self-hosted openpilot-server."),
    "flags": "PERSISTENT",
    "param_type": "STRING",
    "default": "http://lirou.fun:7898",
    "reboot_on_change": True,
  },
]
