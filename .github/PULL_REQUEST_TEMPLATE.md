## Description

<!-- What does this PR do and why? Include PoE/Ethernet quirks or protocol details if relevant. -->

Fixes # <!-- issue number if applicable -->

## Type of Change

- [ ] Bug fix
- [ ] New feature
- [ ] Refactor (no functional change)
- [ ] Documentation
- [ ] Build / CI

## Testing

- [ ] `pio run -e esp32_poe` builds without errors
- [ ] Tested on hardware (board, firmware version)
- [ ] Ethernet link established and IP obtained via DHCP
- [ ] Web UI accessible after boot
- [ ] MQTT / Home Assistant entities verified (if connectivity changes)
- [ ] Alarm state machine exercised (if alarm logic changes)

## Checklist

- [ ] `secrets.h` is **not** included in this PR
- [ ] No hardcoded IPs, passwords, or credentials
- [ ] Backwards-compatible with existing NVS config (or migration documented)
- [ ] CHANGELOG.md updated (if user-visible change)
