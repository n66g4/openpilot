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
    "default": "wss://op.lirou.fun:666",
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
    "default": "https://op.lirou.fun:666",
    "reboot_on_change": True,
  },
]
