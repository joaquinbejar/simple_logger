#  simple logger

EMERGENCY: A panic-level condition necessitating immediate attention from the entire technical team. Affects multiple systems and could have catastrophic consequences (e.g., natural disasters).

ALERT: Issues demanding immediate resolution and notification to the relevant technical staff. Example: Loss of a secondary ISP connection.

CRITICAL: Failures in primary systems requiring immediate attention. Prioritize these over ALERTS. Example: Loss of the primary ISP connection.

ERROR: Non-urgent failures to be relayed to developers or admins; each item should be resolved within a specific timeframe.

WARNING: Cautionary messages indicating potential future errors if action is not taken. Example: File system 85% full.

NOTICE: Unusual events that are not errors but could signal potential issues. These might be summarized and emailed to the technical team.

INFORMATIONAL: Standard operational messages useful for reporting and performance metrics. No immediate action required.

DEBUG: Information aimed at aiding developers in debugging the application; not useful during normal system operations.


