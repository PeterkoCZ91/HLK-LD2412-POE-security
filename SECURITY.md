# Security Policy

## Supported Versions

Only the latest release on `main` receives security fixes.

| Version | Supported |
|---------|-----------|
| latest | Yes |
| older | No |

## Reporting a Vulnerability

**Do not open a public GitHub issue for security vulnerabilities.**

Report issues by email to the maintainer listed on the GitHub profile. Include:

- Description of the vulnerability and potential impact
- Steps to reproduce
- Affected version(s)
- Suggested fix if available

You will receive a response within 7 days. Confirmed issues will be fixed and credited in the changelog unless you prefer to stay anonymous.

## Scope

| Component | Notes |
|-----------|-------|
| Web UI (port 80) | HTTP Basic Auth; default credentials trigger a warning banner |
| MQTT / MQTTS | Optional TLS on port 8883 with CA certificate validation |
| OTA updates | Password-protected endpoint |
| Telegram bot | Command whitelist, chat ID restriction |

**Out of scope:** physical access to the board, LAN8720A / LD2412 module firmware (maintained by their respective manufacturers), third-party Arduino libraries.

## Hardening Notes

- Change default web credentials immediately after first boot
- Use MQTTS (port 8883) in production
- Keep `secrets.h` out of version control (in `.gitignore`)
- The board has no WiFi or BLE — the attack surface is limited to the wired Ethernet interface and MQTT broker
