# Flashing Guide — POE-2412 Security

Flash pre-built firmware onto your ESP32 PoE board without installing PlatformIO.  
Total time: ~5 minutes.

---

## What You Need

- **ESP32 PoE board** with LAN8720A RMII Ethernet (e.g. Prokyber ESP32-STICK Active POE)
- **LD2412** 24 GHz mmWave radar module
- **USB cable** (Micro-USB or USB-C depending on your board) — for first flash only
- **RJ45 Ethernet cable** + PoE injector (IEEE 802.3af) or PoE-capable switch
- **Python 3.8+** on your PC

> After first flash, subsequent updates are done via OTA over Ethernet — no USB needed.

---

## 1. Download Firmware

Go to the [**Releases page**](https://github.com/PeterkoCZ91/POE-2412-security/releases/latest) and download:

```
firmware-poe2412-vX.Y.Z.bin
```

---

## 2. Install esptool

```bash
pip install esptool
```

---

## 3. Wire the LD2412 Sensor

Connect the LD2412 to the ESP32 PoE board before powering on.

```
ESP32 PoE board              HLK-LD2412
────────────────             ──────────
5V  ───────────────────────► VCC
GND ───────────────────────► GND
GPIO 33 (UART RX) ─────────► TX   (radar transmits → ESP32 receives)
GPIO 32 (UART TX) ◄───────── RX   (ESP32 commands → radar)
GPIO 4  (OUT) ◄──────────── OUT  (optional: presence output pin)
```

> **TX/RX cross-over:** GPIO 33 is the ESP32's receive pin, so it connects to the radar's TX. GPIO 32 is the ESP32's transmit pin, so it connects to the radar's RX. This is the most common wiring mistake.

The LD2412 requires a stable 5 V supply. Power it from the board's 5V rail (fed from PoE) or an external 5 V adapter.

---

## 4. Connect Ethernet + PoE

Plug the RJ45 cable into the board. The PoE injector or switch must supply **IEEE 802.3af** (15.4 W). The board does not have a DC barrel jack — PoE is the primary power source in production. For the initial USB flash, power comes from the USB cable.

---

## 5. Flash via USB (First Flash)

Connect the ESP32 board to your PC via USB, then run:

```bash
esptool.py --chip esp32 --port PORT --baud 460800 write_flash 0x0 firmware-poe2412-vX.Y.Z.bin
```

Replace `PORT`:

| OS | Example |
|----|---------|
| Linux | `/dev/ttyUSB0` or `/dev/ttyACM0` |
| macOS | `/dev/cu.usbserial-*` |
| Windows | `COM3` (check Device Manager → Ports) |

**If flashing fails:** hold the **BOOT** button while the command starts, release after "Connecting…" appears.

---

## 6. First Boot

1. Disconnect USB, connect the RJ45 cable to a PoE switch or injector
2. The board obtains an IP address from your DHCP server automatically
3. Find the IP in your router's DHCP client list, or check serial output
4. Open `http://<device-ip>/` in your browser
5. Default credentials: **admin / admin** — change immediately in **Network → Web credentials**

> There is no WiFi access point or captive portal on this board — Ethernet only.

---

## 7. OTA Updates (After First Flash)

Set your device's IP in `platformio.ini` under `[env:ota_poe]`:

```ini
upload_port = 192.168.1.x   ; your device IP
upload_flags = --auth=your_ota_password
```

Then flash:

```bash
pio run -e ota_poe --target upload
```

---

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| No Ethernet link (LED off) | Check PoE injector power, try another cable/port |
| Device not in DHCP table | Wait 30 s after boot; check serial output for Ethernet init errors |
| Web UI unreachable | Verify IP from DHCP table; mDNS may take 30 s |
| Flashing fails | Hold BOOT button; check cable is data-capable (not charge-only) |
| Radar not detected | Verify TX/RX cross-over wiring; confirm 5 V on LD2412 VCC |

---

## Building from Source

See [CONTRIBUTING.md](../CONTRIBUTING.md) for the PlatformIO build setup.
