# rotational-turret
A WiFi-controlled rotational turret built with an ESP8266 (NodeMCU) and two servo motors, as part of Hackclub Blueprint.

## Repository Structure

```
rotational-turret/
├── firmware/           # ESP8266 source code (PlatformIO / Arduino)
│   ├── platformio.ini  # PlatformIO project config
│   └── src/
│       └── main.cpp    # Main sketch – WiFi web-server servo control
└── hardware/           # KiCad PCB design files
    ├── rotational-turret.kicad_pro  # KiCad project
    ├── rotational-turret.kicad_sch  # Schematic
    └── rotational-turret.kicad_pcb  # PCB layout
```

## Firmware

The firmware runs on a NodeMCU (ESP8266) and exposes a simple HTTP server.
Open the board's IP address in a browser to control the **pan** and **tilt** servos with sliders.

### Setup

1. Install [PlatformIO](https://platformio.org/).
2. Copy the example credentials file and fill in your WiFi details:
   ```
   cp firmware/src/secrets.h.example firmware/src/secrets.h
   # then edit secrets.h with your SSID and password
   ```
3. Build & upload:
   ```
   cd firmware
   pio run --target upload
   ```
4. Open the Serial Monitor (`pio device monitor`) to get the assigned IP address.

## Hardware

The PCB is designed in **KiCad 7+**.  Open `hardware/rotational-turret.kicad_pro` to view the schematic and PCB layout.

### Bill of Materials

| Ref | Component | Notes |
|-----|-----------|-------|
| U1  | NodeMCU v1.0 (ESP8266) | Main controller |
| M1  | Standard servo (pan) | Connected to D1 |
| M2  | Standard servo (tilt) | Connected to D2 |
| BT1 | 7.4 V LiPo battery | Via JST connector |
