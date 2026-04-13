# :shield: POE-2412 Security :electric_plug:

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/MCU-ESP32--WROOM--32-blue?logo=espressif)](https://www.espressif.com/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)
[![Version](https://img.shields.io/badge/Version-4.1.3--poe-blue)]()

**Ethernet (PoE) variant** of [HLK-LD2412-security](https://github.com/PeterkoCZ91/HLK-LD2412-security) — standalone intrusion detection system built on ESP32 + HLK-LD2412 24 GHz mmWave radar over **wired Ethernet with Power over Ethernet**. Full alarm state machine, zone management, Home Assistant integration, Telegram bot, and a dark-mode web dashboard. No cloud, no WiFi — one cable for power and data.

> [!TIP]
> **New in v4.1** -- Scheduled arm/disarm, auto-arm after idle, chip temperature monitoring, heap watchdog, and HTTP stability fix (ESP32Async fork).

---

## Table of Contents

- [In 3 Points](#in-3-points)
- [Why PoE?](#why-poe)
- [Who Is This For](#who-is-this-for)
- [What You Need](#what-you-need)
- [Quick Start](#quick-start) (~10 min)
- [How It Works](#how-it-works)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Web Dashboard](#web-dashboard)
- [Telegram Bot](#telegram-bot)
- [API Reference](#api-reference)
- [Differences from WiFi Variant](#differences-from-wifi-variant)
- [Sensor Firmware Quirks](#sensor-firmware-quirks)
- [Known Issues & Limitations](#known-issues--limitations)
- [Troubleshooting](#troubleshooting)
- [Roadmap](#roadmap)
- [FAQ](#faq)
- [Development History](#development-history)
- [Acknowledgments](#acknowledgments)
- [License](#license)

---

## In 3 Points

1. **It's a security system, not a smart home gadget.** No light automations, no room tracking. Every feature answers one question: *does this help detect an intruder?*
2. **It runs over Ethernet.** No wireless signal to jam, no WiFi interference, no dead zones. PoE means one cable for power and data — ideal for ceiling or wall-mounted sensors.
3. **It's battle-tested.** Forked from the WiFi variant (50+ versions, 13 security bugs found and fixed through formal audit). All security features carry over, plus Ethernet-specific hardening.

---

## Why PoE?

| | WiFi | PoE (this project) |
|---|---|---|
| **Cabling** | Power + WiFi (2 things to maintain) | Single Ethernet cable |
| **Reliability** | Signal drops, interference, dead zones | Rock-solid wired connection |
| **Security** | Wireless signal can be jammed | No wireless attack surface |
| **Mounting** | Near outlet + good WiFi coverage | Anywhere you can run a cable |
| **Power** | USB / external 5V adapter | 802.3af/at PoE — no separate PSU |

All features from the [WiFi variant](https://github.com/PeterkoCZ91/HLK-LD2412-security) are preserved: alarm state machine, zones, MQTT auto-discovery, Telegram, web dashboard, OTA with rollback. Plus PoE-specific extras: chip temperature monitoring, heap watchdog, ETH link watchdog.

---

## Who Is This For

- **DIY security enthusiasts** who want wired reliability without cloud subscriptions
- **Home Assistant users** looking for a hardwired radar node with proper alarm state management
- **Vacation home / garage / warehouse owners** who need unattended PoE-powered monitoring
- **PoE infrastructure users** who already have a PoE switch and want to add security nodes with zero extra wiring

---

## What You Need

### Hardware

| Part | Description | ~Cost |
|------|-------------|-------|
| Prokyber ESP32-STICK-PoE-P | ESP32 + LAN8720A RMII Ethernet, active PoE | ~$15--25 |
| HLK-LD2412 | 24 GHz FMCW mmWave radar (UART) | $4--6 |
| PoE switch or injector | 802.3af/at compliant | $15--30 (or existing) |
| Ethernet cable | Cat5e or better | $1--5 |
| **Total** | | **~$35--65** |

> Any ESP32 board with LAN8720A RMII Ethernet should work with pin adjustments in `platformio.ini`. The Prokyber ESP32-STICK-PoE-P is the tested reference board.

### Software (All Free)

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- [Home Assistant](https://www.home-assistant.io/) (optional, for MQTT integration)
- [Telegram](https://telegram.org/) (optional, for mobile alerts)

### Required Skills

- Basic soldering (4 wires: VCC, GND, TX, RX from ESP32 to radar)
- Editing a config file (MQTT server)
- Flashing an ESP32 via USB (first time only — OTA after that)

---

## Quick Start

**~10 minutes from clone to working alarm.**

```bash
# 1. Clone
git clone https://github.com/PeterkoCZ91/POE-2412-security.git
cd POE-2412-security

# 2. Edit include/secrets.h — set your MQTT broker IP
#    (Telegram is optional — leave defaults to skip)

# 3. Wire the radar (see pin table below) and connect ESP32 via USB

# 4. Build and flash
pio run -e esp32_poe --target upload
```

### First Boot

After flashing, the device:

1. Initializes Ethernet (LAN8720A RMII)
2. Waits for link (cable connected to PoE switch)
3. Obtains IP via DHCP (or uses static IP if configured)
4. Connects to MQTT broker (if configured)
5. Starts web dashboard + radar processing

Open the **web dashboard**:
- URL: `http://<device-ip>/` or `http://<hostname>.local/`
- **Default login:** `admin` / `admin` (change it in Network & Cloud tab!)

> [!NOTE]
> **Don't know your device's IP?** Check your PoE switch/router DHCP leases, or open the serial monitor (`pio device monitor`) and look for `[ETH] IP: x.x.x.x`.

### Pin Layout

| ESP32 GPIO | LD2412 Pin | Function | Note |
|------------|------------|----------|------|
| GPIO 33 | TX | UART RX | ESP32 receives radar data |
| GPIO 32 | RX | UART TX | ESP32 sends commands |
| GPIO 4 | OUT | Digital output | Optional presence signal |
| 5V | VCC | Power | From ESP32 5V rail (PoE powered) |
| GND | GND | Ground | |

> **Important:** GPIO 18, 19, 21, 22, 23, 25, 26, 27 are reserved for Ethernet (LAN8720A RMII) and **cannot** be used for anything else. This is why the radar uses GPIO 32/33 instead of the WiFi variant's GPIO 18/19.

<details>
<summary><strong>Full Prokyber ESP32-STICK-PoE-P pin map</strong></summary>

| Pin | Function | Note |
|------|------------------|----------------------------------------|
| GPIO 0 | ETH CLK IN | EMAC_TX_CLK (LAN8720A → ESP32) |
| GPIO 2 | USER LED | Status LED |
| GPIO 4 | RADAR_OUT_PIN | Digital presence output |
| GPIO 17 | ETH CLK OUT | Clock output to LAN8720A |
| GPIO 18 | ETH_MDIO | RESERVED for LAN8720A |
| GPIO 19 | EMAC_TXD0 | RESERVED for LAN8720A |
| GPIO 21 | EMAC_TX_EN | RESERVED for LAN8720A |
| GPIO 22 | EMAC_TXD1 | RESERVED for LAN8720A |
| GPIO 23 | ETH_MDC | RESERVED for LAN8720A |
| GPIO 25 | EMAC_RXD0 | RESERVED for LAN8720A |
| GPIO 26 | EMAC_RXD1 | RESERVED for LAN8720A |
| GPIO 27 | EMAC_CRS_DV | RESERVED for LAN8720A |
| GPIO 32 | RADAR_RX | ← LD2412 TX |
| GPIO 33 | RADAR_TX | → LD2412 RX |

</details>

---

## How It Works

The LD2412 radar continuously scans a 60-degree cone at 24 GHz, reporting distance (0--9 m), motion energy, and static energy for each of its 14 detection gates (0.75 m per gate). The ESP32 reads these frames over UART, runs them through a security-focused processing pipeline, and makes alarm decisions.

```
  LD2412 Radar          ESP32 Processing Pipeline           Outputs
 +-----------+     +-------------------------------+    +-----------+
 |  24 GHz   | UART|  Frame Parser (state machine) |    |  MQTT/HA  |
 |  FMCW     |---->|  Pet Immunity Filter          |    |  Telegram |
 |  Scanner   |    |  Zone Classifier              |--->|  Webhook  |
 |  14 gates  |    |  Alarm State Machine          |    |  Siren    |
 |  ~5 Hz     |    |  Approach Tracker             |    |  Web UI   |
 +-----------+     +-------------------------------+    +-----------+
                          |
                   [Ethernet/PoE]
                   No WiFi. No wireless.
```

### Alarm State Machine

```
                    arm (with delay)
  DISARMED ──────────────────────> ARMING
      ^                              |
      |  disarm                      | exit delay expires
      |                              v
      +<─────── TRIGGERED <──── ARMED
                    ^               |
                    |               | detection in zone
                    | entry delay   v
                    +────────── PENDING
```

States: **DISARMED** -> **ARMING** (exit delay) -> **ARMED** -> **PENDING** (entry delay) -> **TRIGGERED** -> auto-rearm or disarm.

---

## Features

### :lock: Security
| Feature | Description |
|---------|-------------|
| Alarm state machine | 5 states with configurable transitions |
| Detection zones | Up to 16 zones with min/max range, per-zone behavior (entry delay / immediate / ignore) |
| Entry & exit delays | Default 30 s each, configurable per-zone |
| Approach tracker | Circular buffer of last 16 armed detections for forensic review |
| Auto-rearm | Re-arms after trigger timeout (default 15 min) |
| Anti-masking | Detects prolonged zero-energy (sensor obstruction or tampering) |
| Pet immunity | Configurable energy threshold filter |
| Loitering detection | Alert when target lingers in close zone beyond timeout |
| Siren/strobe output | Optional GPIO for audible/visual alarm |
| Alarm energy threshold | Minimum energy + debounce before alarm transition |

### :electric_plug: Connectivity
| Feature | Description |
|---------|-------------|
| Ethernet (PoE) | LAN8720A RMII, 100 Mbps, 802.3af/at |
| MQTT + Home Assistant | Auto-discovery, all entities created automatically |
| MQTT offline buffer | Queues messages to LittleFS when broker unreachable |
| MQTTS (TLS) | Optional encrypted MQTT |
| Telegram bot | 12 commands: arm, disarm, status, learn, mute, restart... |
| Discord / webhook | Generic HTTP POST notifications |
| ETH link watchdog | Auto-reboot after 5 min link down |
| Scheduled arm/disarm | Cron-like scheduling with timezone support |
| Auto-arm | Arms automatically after configurable idle period |

### :bar_chart: Diagnostics
| Feature | Description |
|---------|-------------|
| UART health monitoring | Frame rate, error count, resync stats |
| Radar health score | 0--100 composite score |
| Chip temperature | Configurable warning/critical thresholds with Telegram alerts |
| Heap watchdog | Low/critical RAM alerts with auto-recovery |
| Dead Man's Switch | Auto-restart if no MQTT publish in 30 min |
| Event log | 100 events on LittleFS with CSV export |
| Heap monitoring | Free/min/max-alloc with low-memory warnings |

### :computer: Interface
| Feature | Description |
|---------|-------------|
| Web dashboard | Responsive dark-mode UI with real-time SSE |
| REST API | `/api/health`, `/api/telemetry`, `/api/config` + 50 more |
| Engineering mode | Per-gate energy visualization via MQTT |
| mDNS | `http://hostname.local/` access |

### :shield: Reliability
| Feature | Description |
|---------|-------------|
| OTA updates | With automatic rollback (60 s validation) |
| Watchdog | 60 s hardware WDT |
| NVS persistence | All config, uptime, alarm state survives reboot |
| Gate config verification | Post-boot check to catch firmware config reverts |
| Config snapshot | Auto-backup NVS config to LittleFS before OTA |

---

## Web Dashboard

Dark-mode responsive web UI accessible at `http://<device-ip>/`. Tabs for:

- **Status** -- real-time distance, energy, alarm state, radar health, chip temperature
- **Security** -- zones, delays, anti-masking, loitering, pet immunity
- **Gates** -- per-gate sensitivity with presets (indoor/outdoor/pet)
- **Network** -- MQTT, Telegram, webhook config (no WiFi section)
- **Zones** -- visual zone editor with distance map
- **History** -- event log with CSV export

> The entire UI is embedded as a single PROGMEM string (~50 KB). No external files, no SD card needed.

---

## Telegram Bot

Create a bot via [@BotFather](https://t.me/BotFather), add token and chat ID to `secrets.h`.

| Command | Description |
|---------|-------------|
| `/start` | Welcome + quick command list |
| `/arm` | Arm with exit delay |
| `/arm_now` | Arm immediately |
| `/disarm` | Disarm |
| `/status` | State, distance, energy, uptime, heap, chip temp |
| `/light` | Ambient light reading (eng mode required) |
| `/learn` | Start static reflector learning (3 min) |
| `/eng_on` `/eng_off` | Toggle engineering mode |
| `/mute` `/unmute` | Notification control |
| `/restart` | Restart ESP32 |

---

## API Reference

### Endpoints

All endpoints from the [WiFi variant](https://github.com/PeterkoCZ91/HLK-LD2412-security#api-reference) are available.

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/health` | GET | Uptime, Ethernet info, MQTT, heap, hostname, reset history, chip temp |
| `/api/telemetry` | GET | Radar state, distance, energy, direction, UART stats |
| `/api/config` | GET/POST | Full configuration read/write |
| `/api/preset?name=indoor` | POST | Apply sensitivity preset |
| `/api/zones` | GET/POST | Zone configuration |
| `/api/events` | GET | Event log (JSON) |
| `/api/events/csv` | GET | Event log export (CSV) |
| `/api/schedule` | GET/POST | Scheduled arm/disarm times |
| `/api/timezone` | GET/POST | Timezone and DST offsets |
| `/api/arm` | POST | Arm the system |
| `/api/disarm` | POST | Disarm the system |

**PoE-specific in `/api/health`:**
```json
{
  "ethernet": {
    "link_up": true,
    "ip": "192.168.1.100",
    "mac": "AA:BB:CC:DD:EE:FF",
    "speed": 100
  },
  "chip_temp": 52.3
}
```

> **Removed:** `/api/wifi/config` (no WiFi on this board)

### MQTT Topics

Prefix: `security/<device_id>/`

| Topic | Direction | Description |
|-------|-----------|-------------|
| `presence/state` | publish | IDLE / DETECTED / HOLDING / TAMPER |
| `presence/distance` | publish | Distance in cm |
| `presence/energy_mov` | publish | Moving energy (0--100) |
| `presence/energy_stat` | publish | Static energy (0--100) |
| `presence/direction` | publish | approaching / receding / static |
| `alarm/state` | publish | DISARMED / ARMING / ARMED / PENDING / TRIGGERED |
| `alarm/set` | subscribe | `arm` / `disarm` / `arm_now` |
| `alarm/event` | publish | JSON alarm trigger event |
| `alert/tamper` | publish | Tamper/anti-masking alert |
| `alert/loitering` | publish | Loitering alert |
| `diag/health_score` | publish | Radar health 0--100 |
| `diag/heap_free` | publish | Free heap KB |
| `diag/chip_temp` | publish | Chip temperature °C |
| `diag/eth_link` | publish | Ethernet link status |
| `availability` | publish | online / offline (LWT) |

---

## Differences from WiFi Variant

| Area | WiFi variant | PoE variant |
|------|-------------|-------------|
| Connectivity | WiFi + backup SSID + AP portal | Ethernet (LAN8720A RMII) |
| Power | USB / external 5V | PoE (802.3af/at) via Ethernet |
| UART pins | GPIO 16/17 or 18/19 | GPIO 32/33 (ETH occupies 18/19) |
| RSSI monitoring | WiFi signal + jamming detection | N/A (wired) |
| Provisioning | WiFiManager AP portal + BLE fallback | Static config in `secrets.h` |
| `/api/health` | `wifi.rssi`, `wifi.ssid` | `ethernet.link_up/ip/mac/speed` |
| STAB log | RSSI value | `ETH:UP/DOWN` |
| SSE telemetry | `rssi` field | `eth_link` field |
| Chip temp monitoring | Not included | Warning + critical Telegram alerts |
| Heap watchdog | Not included | Low/critical RAM Telegram alerts |
| ETH link watchdog | N/A | Auto-reboot after 5 min link down |
| Flash size | 4 MB | 16 MB |
| Version scheme | v3.x.x | v4.x.x-poe |

Everything else is identical: alarm state machine, detection zones, MQTT auto-discovery, Telegram bot, web dashboard, event logging, OTA with rollback.

---

## System Architecture

```
src/
 +-- main.cpp                  Main loop, Ethernet, OTA, telemetry publishing
 +-- WebRoutes.cpp             REST API + SSE endpoints
 +-- ConfigManager.cpp         NVS persistence layer
 +-- services/
      +-- LD2412Service.cpp    Radar driver (UART, state machine, recovery)
      +-- SecurityMonitor.cpp  Alarm logic, zones, anti-masking, approach tracker
      +-- MQTTService.cpp      HA auto-discovery, publish/subscribe
      +-- MQTTOfflineBuffer.cpp Flash-backed message queue
      +-- TelegramService.cpp  Bot polling + command handler
      +-- NotificationService.cpp Webhook + Discord dispatcher
      +-- EventLog.cpp         LittleFS ring buffer (100 events)
      +-- LogService.cpp       In-memory system log
      +-- ConfigSnapshot.cpp   Auto-backup NVS before OTA
      +-- BluetoothService.cpp Emergency BLE provisioning

lib/
 +-- LD2412_Extended/          Low-level UART protocol driver
      +-- LD2412.cpp           Frame parser, command interface, ACK verification
      +-- LD2412.h             Protocol constants, buffer management

include/
 +-- constants.h               All timing, thresholds, deadbands
 +-- web_interface.h           Embedded HTML/CSS/JS dashboard (~50 KB)
 +-- secrets.h                 Credential configuration
 +-- known_devices.h           Multi-device MAC mapping
```

### Data Flow

```
  UART RX (5 Hz)
       |
  [LD2412.cpp] Frame parser with state machine + length validation
       |
  [LD2412Service.cpp] State tracking, direction inference, health monitoring
       |
  [SecurityMonitor.cpp] Zone classification -> alarm state machine -> event queue
       |
  [main.cpp] Dispatches to:
       +-- MQTTService (HA auto-discovery, deadband publish)
       +-- TelegramService (bot commands)
       +-- NotificationService (webhooks)
       +-- EventLog (LittleFS persistence)
       +-- WebRoutes (SSE real-time stream)
       |
  [Ethernet/PoE] All network traffic over wired connection
```

---

## Sensor Firmware Quirks

> [!WARNING]
> The HLK-LD2412 has significant firmware-dependent behavior. **V1.26 is the most common version** shipped on new modules, and it has multiple UART command bugs.

| Feature | V1.24 | V1.26 | V1.36 |
|---------|:-----:|:-----:|:-----:|
| Basic frame output | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| Engineering mode (UART) | :x: ACKs, doesn't switch | :x: ACKs, doesn't switch | :white_check_mark: |
| setBaudRate (UART) | :x: ACKs, doesn't switch | :x: ACKs, doesn't switch | ? |
| setResolution (UART) | :x: Breaks radar | :x: Breaks radar | ? |
| FW query (0xA0) | :x: Returns 0 | :white_check_mark: | :white_check_mark: |
| Gate config persistence | :warning: May revert | :warning: May revert after ~30s | :white_check_mark: |

**Built-in workarounds:**
- Baud rate upgrade skipped on V1.26
- Engineering mode auto-disabled on V1.26
- Gate config verified 40s post-boot (catches silent reverts, see [ESPHome #13366](https://github.com/esphome/issues/issues/13366))
- Resolution changes: use HLKRadarTool Bluetooth app (code: **NUNIXB**)

### How to Update / Downgrade Sensor Firmware

1. **Install HLKRadarTool** -- [Android](https://play.google.com/store/apps/details?id=com.hlk.hlkradartool) / [iOS](https://apps.apple.com/app/hlkradartool/id6475738581)
2. **Power cycle** the LD2412 module (BLE only activates for ~30s after power-on)
3. **Connect** via Bluetooth in HLKRadarTool (device: `HLK-LD2412_XXXX`)
4. **Enter firmware code:** `NUNIXB`
5. **Select version** and flash (~30s, do not power off)
6. **Power cycle** and **verify** via `GET /api/health` → `fw_version`

---

## Known Issues & Limitations

<details>
<summary><strong>Hardware / Firmware (won't fix)</strong></summary>

- Engineering mode over UART broken on V1.24 and V1.26 -- use HLKRadarTool Bluetooth app
- `setResolution()` can brick radar detection on some FW -- only change via Bluetooth
- Radar may not start when powered simultaneously with ESP32 -- power cycle the radar module
- V1.36 (only FW with working eng mode) not available via standard update channel
- GPIO 18/19/21/22/23/25/26/27 are reserved for Ethernet and cannot be repurposed

</details>

<details>
<summary><strong>Software limitations</strong></summary>

- No WiFi fallback -- if Ethernet cable is disconnected, device is unreachable until reconnected
- MQTT offline buffer limited to ~20 messages (LittleFS constraint)
- Web UI is a ~50 KB PROGMEM string -- editing requires firmware rebuild
- Scheduled arm/disarm requires NTP sync (no internet = no schedule)
- Multi-sensor supervision heartbeat and mesh alarm verification via MQTT (zero-config, auto-discovery)
- `resync_count` in diagnostics looks alarming but ~20 resyncs/frame is **normal** parser behavior

</details>

---

## Troubleshooting

<details>
<summary><strong>No radar detection after flashing</strong></summary>

**Symptoms:** Web dashboard shows distance 0, energy 0. Serial monitor shows `[RADAR] Failed to init LD2412`.

**Causes & fixes:**
1. **Wiring error** -- RX/TX are crossed: ESP32 GPIO 33 connects to LD2412 TX, GPIO 32 to LD2412 RX
2. **Power issue** -- LD2412 needs stable 5V; verify PoE supply is sufficient
3. **Radar not starting** -- power cycle the radar module (disconnect 5V briefly)

</details>

<details>
<summary><strong>No Ethernet connection</strong></summary>

**Check:**
1. Ethernet cable is firmly connected to both ESP32 board and PoE switch
2. PoE switch/injector is providing power (check LED indicators)
3. Serial monitor shows `[ETH] Link UP` -- if not, check cable and switch port
4. DHCP server is running on your network

**If using static IP:** Configure via web dashboard or `secrets.h`.

</details>

<details>
<summary><strong>MQTT not connecting / no Home Assistant entities</strong></summary>

**Check:**
1. MQTT server IP is correct in `secrets.h` or web dashboard (Network & Cloud tab)
2. MQTT broker is running and reachable from device's network
3. Serial monitor shows `[MQTT] Connected` -- if not, check broker logs

**Note:** The device works fully standalone without MQTT. Events are buffered to flash and replayed when MQTT reconnects.

</details>

<details>
<summary><strong>Telegram bot not sending messages</strong></summary>

If the Telegram token in `secrets.h` is empty, the bot is **silently disabled**. To verify:
1. Serial monitor at boot: `[Telegram] Bot initialized` = working, no message = disabled
2. `GET /api/health` -- check `telegram_enabled`
3. Verify token from [@BotFather](https://t.me/BotFather) and chat ID from [@userinfobot](https://t.me/userinfobot)

</details>

<details>
<summary><strong>False alarms / too sensitive</strong></summary>

1. Apply a sensitivity preset: **Indoor** / **Outdoor** / **Pet** (Gates tab or `POST /api/preset?name=indoor`)
2. Increase **pet immunity threshold** (Security tab)
3. Use **static zone learning** (`/learn` via Telegram) to identify furniture reflectors
4. Adjust **zone ranges** to exclude areas outside your space

</details>

<details>
<summary><strong>Chip temperature warnings</strong></summary>

The PoE variant monitors ESP32 chip temperature. If you receive temperature alerts:
1. Check ventilation around the device
2. Verify PoE voltage is within spec (48V nominal)
3. Thresholds are configurable via web dashboard (default: warn 70°C, critical 85°C)

</details>

---

## Roadmap

| Feature | Status | Description |
|---------|--------|-------------|
| Multi-sensor mesh | :white_check_mark: v4.2.0 | Nodes cross-verify alarms via MQTT. On trigger, peers confirm presence within 5s. Alarm event includes `mesh_verified` field |
| Supervision heartbeat | :white_check_mark: v4.2.0 | Nodes publish alive signal every 60s. If a peer goes silent for 3 min, tamper alert is sent. Auto-discovery of up to 8 peers |
| Entry/exit path logic | :bulb: Planned | Define valid entry paths (door → hallway → room). Entry delay only on correct sequence; wrong path (e.g. window) = immediate trigger |
| Event timeline UI | :bulb: Planned | Visual timeline in web dashboard replacing the table view — easier forensic review of detection events |
| Camera trigger | :bulb: Planned | Snapshot IP camera on alarm event via MQTT/HTTP |
| Static IP via web UI | :bulb: Planned | Configure static IP without reflashing |
| WiFi variant sync | :arrows_counterclockwise: Ongoing | Port new features from WiFi v3.x to PoE v4.x |
| ESP32Async libraries | :white_check_mark: Done | Replaced abandoned me-no-dev/* with community ESP32Async/* (v4.1.3) |
| Chip temp monitoring | :white_check_mark: Done | Warning/critical alerts via MQTT + Telegram (v4.0.2) |
| Heap watchdog | :white_check_mark: Done | Low/critical RAM alerts (v4.0.5) |
| OTA rollback | :white_check_mark: Done | 60s validation with auto-revert (v4.0.2) |
| Scheduled arm/disarm | :white_check_mark: Done | Cron-like with timezone (v4.1.0) |
| MQTT offline buffer | :white_check_mark: Done | LittleFS message queue (v3.9.8-poe) |
| ETH link watchdog | :white_check_mark: Done | Auto-reboot after 5 min link down |

---

## FAQ

<details>
<summary><strong>Do I need Home Assistant?</strong></summary>

No. The system works fully standalone with the web dashboard and Telegram bot. Home Assistant adds remote control via MQTT and nice dashboards, but it's optional.

</details>

<details>
<summary><strong>Can I use a regular ESP32 board with an Ethernet module?</strong></summary>

Potentially yes, if your board uses LAN8720A with RMII interface. You'll need to adjust the ETH pin definitions in `platformio.ini`. The Prokyber ESP32-STICK-PoE-P is the tested reference, but any ESP32 + LAN8720A combo should work with correct pin mapping.

</details>

<details>
<summary><strong>What happens when the Ethernet cable is disconnected?</strong></summary>

The alarm keeps running locally. Events are logged to flash (EventLog) and MQTT messages are queued in the offline buffer. The ETH link watchdog auto-reboots after 5 minutes of link down to attempt recovery. When connectivity returns, buffered messages are replayed.

</details>

<details>
<summary><strong>Can I use multiple PoE sensors?</strong></summary>

Yes. Flash the same firmware to multiple boards — each identifies itself by MAC address (via `known_devices.h`) and gets its own MQTT topics. Add OTA environments per device in `platformio.ini`. Since v4.2.0, nodes automatically discover each other via MQTT, monitor peer health (supervision heartbeat), and cross-verify alarms (mesh verification).

</details>

<details>
<summary><strong>How much power does it draw?</strong></summary>

The ESP32 + LD2412 combination draws roughly 2--3W. Well within 802.3af limits (12.95W per port). A typical 8-port PoE switch can power dozens of these nodes.

</details>

<details>
<summary><strong>Why v4.x instead of v3.x?</strong></summary>

The PoE variant uses v4.x versioning to clearly distinguish it from the WiFi variant (v3.x). The feature set is kept in sync, with PoE-specific additions (chip temp, heap watchdog, ETH link monitoring) layered on top.

</details>

---

## Development History

Forked from the WiFi variant at v3.6.8, then evolved independently with Ethernet-specific features.

| Phase | Versions | Focus |
|-------|----------|-------|
| Fork & Port | v3.6.8--3.9.x-poe | WiFi → Ethernet, pin remapping, ETH link watchdog |
| PoE Hardening | v4.0.x-poe | OTA rollback, chip temp monitoring, heap watchdog, config snapshot |
| Feature Sync | v4.1.x-poe | Scheduled arm/disarm, auto-arm, ESPHome hardening, ESP32Async libs |

---

## Acknowledgments

- [HLK-LD2412-security](https://github.com/PeterkoCZ91/HLK-LD2412-security) -- the WiFi variant this project is forked from
- [HLK-LD2412 Protocol V1.05](https://assets.super.so/79c0d2a8-d37a-438f-8fbe-c44778f3b0dd/files/6b5abbe3-d359-4ef3-a125-61b32b1620b1/HLK-LD2412_Serial_Communication_Protocol_V1.05_240808.pdf) -- official serial protocol documentation
- [tobiastl/LD2412](https://github.com/tobiastl/LD2412) -- Arduino library (base for our extended driver)
- [ESP32Async](https://github.com/ESP32Async) -- maintained community fork of AsyncTCP + ESPAsyncWebServer
- ESPHome community -- protocol insights, firmware compatibility testing, security audit patterns

---

## License

MIT License. See [LICENSE](LICENSE) for details.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Keep commits focused and test your changes
4. Open a pull request with a clear description

Bug reports and feature requests are welcome via [GitHub Issues](../../issues).
