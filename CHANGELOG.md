# Changelog

All notable changes to the POE-2412 Security System firmware are documented in this file.
Format based on [Keep a Changelog](https://keepachangelog.com/).

## [v4.1.3-poe] - 2026-04-12
### Fixed
- **HTTP stability**: Replaced abandoned `me-no-dev/AsyncTCP` + `ESPAsyncWebServer` with community-maintained `ESP32Async/*` fork. The old libraries had race conditions in digest auth parser and TCP close handling, causing alternating 200/401/empty-reply responses under repeated requests (worse on Ethernet due to ESP-IDF `tcpip_thread`). API drop-in compatible.

## [v4.1.2-poe] - 2026-04-10
### Fixed
- **Live SSE telemetry broken**: SSE buffer was 256 B but telemetry JSON is ~700-900 B. Buffer increased to 1024 B. Regression from LD2412 v3.10.0 port.

## [v4.1.1-poe] - 2026-04-04
### Fixed
- Gate config verification 40s post-boot (ESPHome #13366 V1.26 revert workaround)
- `processRadarData` mutex timeout increased 10ms to 50ms
- Alarm event queue overflow alerting
- `enableConfig` serial drain before retry
- `getAckNonBlocking` early length validation

## [v4.1.0-poe] - 2026-04-03
### Added
- **Scheduled arm/disarm**: time-based HH:MM scheduling with timezone support
- **Auto-arm**: configurable idle timeout (no presence for N minutes → auto-arm)
- **Timezone**: configurable `tz_offset`/`dst_offset`
- **CSV export**: `GET /api/events/csv` endpoint
- **Static IP support**

### Changed
- SSE telemetry: `String` replaced with stack `char[]` (less heap fragmentation)
- MQTT: `String(val)` replaced with `snprintf`
- SecurityMonitor mutex timeout increased to 200ms

## [v4.0.6-poe] - 2026-03-28
### Added
- Heap diagnostics: free/min/max-alloc tracking
- Crash guards with `std::nothrow` allocations
- Bounds validation on all MQTT sensitivity parameters

## [v4.0.5-poe] - 2026-03-27
### Added
- **Telegram alerts for low RAM**: warning and critical thresholds with auto-recovery notification

## [v4.0.4-poe] - 2026-03-26
### Added
- **Telegram alerts for chip temperature**: warning and critical thresholds

## [v4.0.3-poe] - 2026-03-25
### Added
- Chip temperature MQTT publishing with configurable interval

## [v4.0.2-poe] - 2026-03-24
### Added
- **OTA rollback**: 60-second validation window with automatic revert
- Chip temperature reading and reporting

## [v4.0.1-poe] - 2026-03-23
### Fixed
- Removed unsafe PHY LED hack, reverted to stable ETH state

## [v4.0.0-poe] - 2026-03-22
### Added
- PHY LED control experiment (LAN8720A via MDIO) — reverted in v4.0.1

## [v3.9.9-poe] - 2026-03-21
### Added
- Web assets on LittleFS with PROGMEM fallback

## [v3.9.8-poe] - 2026-03-20
### Added
- **MQTT offline buffer**: queue messages to LittleFS when broker disconnected

## [v3.9.7-poe] - 2026-03-20
### Added
- EventLog: persistent disk ring buffer (100 events)

## [v3.9.6-poe] - 2026-03-19
### Added
- **ConfigSnapshot**: auto-backup NVS config to LittleFS before OTA
- 16 MB flash support, OTA partition fix

## [v3.9.5-poe] - 2026-03-18
### Fixed
- Dead Man's Switch: force MQTT reconnect before restart

## [v3.9.3-poe] - 2026-03-17
### Fixed
- Synced with LD2412 v3.9.3 security audit fixes

## [v3.8.5-poe] - 2026-03-15
### Fixed
- Synced with LD2412 v3.8.5

## [v3.7.5-poe] - 2026-03-12
### Added
- Telegram notifications for ETH link watchdog
- ETH link watchdog — auto-reboot after 5 min link down
- Removed WiFi-specific RSSI endpoints

## [v3.6.8-poe] - 2026-03-10
### Added
- **Initial fork** from LD2412 WiFi variant v3.6.8
- Replaced WiFiManager with Ethernet (LAN8720A RMII)
- New UART pins: GPIO 32/33 (Ethernet occupies 18/19)
- Removed backup WiFi, RSSI monitoring, AP portal
