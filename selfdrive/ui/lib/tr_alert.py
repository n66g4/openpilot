import re

from openpilot.system.ui.lib.multilang import tr, tr_noop

# Dynamic alert patterns: (regex, format_msgid)
# format_msgid uses {} placeholders matching regex capture groups.
_DYNAMIC_ALERT_PATTERNS: list[tuple[re.Pattern[str], str]] = [
  (re.compile(r'^Drive above (.+) to engage$'), tr_noop("Drive above {} to engage")),
  (re.compile(r'^Steer Assist Unavailable Below (.+)$'), tr_noop("Steer Assist Unavailable Below {}")),
  (re.compile(r'^Drive Above (.+)$'), tr_noop("Drive Above {}")),
  (re.compile(r'^(\d+) second remaining\. Press again to save early\.$'), tr_noop("{} second remaining. Press again to save early.")),
  (re.compile(r'^(\d+) seconds remaining\. Press again to save early\.$'), tr_noop("{} seconds remaining. Press again to save early.")),
  (re.compile(r'^(\d+)% full$'), tr_noop("{}% full")),
  (re.compile(r'^Speed Error: ([-\d.]+) m/s$'), tr_noop("Speed Error: {} m/s")),
  (re.compile(r'^Remount Device \(Pitch: ([-\d.]+)°, Yaw: ([-\d.]+)°\)$'), tr_noop("Remount Device (Pitch: {}°, Yaw: {}°)")),
  (re.compile(r'^Angle offset too high \(Offset: ([-\d.]+)°\)$'), tr_noop("Angle offset too high (Offset: {}°)")),
  (re.compile(r'^Steering rack geometry may be off \(Ratio: ([-\d.]+)\)$'), tr_noop("Steering rack geometry may be off (Ratio: {})")),
  (re.compile(r'^Check tires, pressure, or alignment \(Factor: ([-\d.]+)\)$'), tr_noop("Check tires, pressure, or alignment (Factor: {})")),
  (re.compile(r'^([-\d.]+) °C$'), tr_noop("{} °C")),
  (re.compile(r'^(\d+)% used$'), tr_noop("{}% used")),
  (re.compile(r'^([-\d.]+)% frames dropped$'), tr_noop("{}% frames dropped")),
  (re.compile(r'^Gas: (\d+)%, Steer: (\d+)%$'), tr_noop("Gas: {}%, Steer: {}%")),
  (re.compile(r'^Driving Personality: (.+)$'), tr_noop("Driving Personality: {}")),
  (re.compile(r'^Calibrating: (\d+)%$'), tr_noop("Calibrating: {}%")),
  (re.compile(r'^Recalibrating: (\d+)%$'), tr_noop("Recalibrating: {}%")),
]

_OFFROAD_EXTRA_TRANSLATIONS = {
  "longitudinal": "纵向",
  "lateral": "横向",
}


def tr_alert(text: str) -> str:
  if not text:
    return text

  translated = tr(text)
  if translated != text:
    return translated

  for pattern, fmt_msgid in _DYNAMIC_ALERT_PATTERNS:
    match = pattern.match(text)
    if match:
      fmt = tr(fmt_msgid)
      return fmt.format(*match.groups())

  return text


def tr_offroad_alert(template: str, extra: str = "") -> str:
  if not template:
    return template
  extra = _OFFROAD_EXTRA_TRANSLATIONS.get(extra, extra)
  return tr(template).replace("%1", extra)
