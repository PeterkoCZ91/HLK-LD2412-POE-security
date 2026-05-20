# Contributing to POE-2412 Security

Bug reports, feature requests, and pull requests are welcome.

---

## Development Setup

**Requirements:** [PlatformIO](https://platformio.org/) (VS Code extension or CLI), Python 3.8+.

```bash
git clone https://github.com/PeterkoCZ91/POE-2412-security.git
cd POE-2412-security

# Config file (never commit this)
cp include/secrets.h.example include/secrets.h
# Edit secrets.h with your MQTT, Telegram, and OTA credentials
```

---

## Build

| Command | Description |
|---------|-------------|
| `pio run -e esp32_poe` | Build for USB first-flash |
| `pio run -e esp32_poe --target upload` | Flash via USB |
| `pio run -e ota_poe --target upload` | Flash via OTA (set IP in platformio.ini first) |

No unit tests exist for this project — manual hardware testing is required.

---

## PoE-Specific Notes

- This board uses **LAN8720A RMII Ethernet** — there is no WiFi. The captive portal and BLE configuration are absent.
- Ethernet initialisation must complete before any network service starts. If you add or modify service startup order, verify that `ETH.begin()` has returned `true` before MQTT or web server initialisation.
- The board uses a **16 MB flash** with a custom partition scheme (`partitions_16mb.csv`). Do not replace it with the standard `min_spiffs.csv`.
- Power is supplied via PoE (IEEE 802.3af). A PoE injector or PoE switch is required — the board does not have a barrel jack.

---

## Branch Naming

| Type | Pattern | Example |
|------|---------|---------|
| New feature | `feature/<description>` | `feature/eth-link-watchdog` |
| Bug fix | `fix/<description>` | `fix/mqtt-reconnect-loop` |
| Documentation | `docs/<description>` | `docs/wiring-lan8720` |
| Refactor | `refactor/<description>` | `refactor/config-manager` |

---

## What to Test Before Opening a PR

- `pio run -e esp32_poe` builds cleanly (zero errors)
- Device boots, obtains DHCP lease, and web UI is reachable
- MQTT connects and Home Assistant entities appear (if networking touched)
- Alarm state machine cycles correctly (if alarm logic touched)

---

## What We're Looking For

- Bug fixes with a clear root-cause description
- Ethernet stability improvements
- Features that fit the embedded + no-WiFi constraints
- Documentation fixes

## What We're Not Looking For

- WiFi or BLE additions — this board has no WiFi
- Anything that changes the partition scheme without updating `partitions_16mb.csv`
- Cloud integrations or external service dependencies
- Changes that commit `secrets.h`

---

## Code Style

- **4 spaces**, no tabs
- `camelCase` for functions/variables, `PascalCase` for classes, `UPPER_SNAKE_CASE` for macros/constants
- No dynamic allocation in hot paths (`new`/`delete` only at init time)
- No `Serial.print` outside `#ifdef SERIAL_DEBUG` guards
- `#pragma once` in all headers
- `secrets.h` must never appear in a commit (in `.gitignore`)
