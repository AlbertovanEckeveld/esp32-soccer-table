# ESP32 Soccer Table Controller

A smart soccer table (foosball) controller built with ESP32 that features automatic goal detection using IR sensors and LED strip celebrations with team-specific colors.

## 🏆 Features

- **Automatic Goal Detection**: IR sensors in each goal detect when a ball passes through
- **LED Strip Celebrations**: 5-meter WS2812B LED strip displays team-specific color waves when goals are scored
- **Non-blocking Architecture**: All systems run simultaneously without delays
- **Team Colors**: Team A (Yellow) vs Team B (Orange) celebrations

## ⚽ How It Works

1. **Normal Mode**: LED strip cycles through beautiful demo effects (white, color waves, rainbow, breathing)
2. **Goal Detection**: IR sensors detect when the ball blocks the beam in either goal
3. **Celebration**: Immediate 3-second team-colored LED celebration wave
4. **Return to Normal**: Demo effects resume after celebration

## 🔧 Hardware Requirements

### Components
- **ESP32 Development Board** (ESP32-DevKit or similar)
- **WS2812B LED Strip** - 5 meters, 60 LEDs/meter (300 total LEDs)
- **2x IR Reflective Sensors** - For goal detection (e.g., TCRT5000)
- **5V Power Supply** - For LED strip (calculate based on LED count)
- **Jumper Wires** and **Breadboard**

### LED Strip Layout
The 5-meter strip is divided into 4 sections around the table perimeter:
- Section 1: 120cm (72 LEDs) - LEDs 0-71
- Section 2: 70cm (42 LEDs) - LEDs 72-113  
- Section 3: 120cm (72 LEDs) - LEDs 114-185
- Section 4: 70cm (42 LEDs) - LEDs 186-227
- Unused: 72 LEDs (228-299)

## 🔌 Wiring Diagram

```
ESP32 Pin 2  → LED Strip Data In
ESP32 Pin 18 → IR Sensor Goal 1 (Team A - Red)
ESP32 Pin 19 → IR Sensor Goal 2 (Team B - Blue)
5V Supply    → LED Strip VCC
GND          → LED Strip GND + IR Sensors GND + ESP32 GND
3.3V         → IR Sensors VCC
```

## 📁 Project Structure

```
esp32-soccer-table/
├── platformio.ini          # PlatformIO configuration
├── include/
│   ├── led-controller.h    # LED strip control declarations
│   └── ir-controller.h     # IR sensor declarations
├── src/
│   ├── main.cpp            # Main application code
│   ├── led-controller.cpp  # LED strip implementation
│   └── ir-controller.cpp   # IR sensor implementation
└── README.md               # This file
```

## 🚀 Installation & Setup

### 1. Platform Setup
- Install [PlatformIO IDE](https://platformio.org/platformio-ide) or use PlatformIO extension in VS Code
- Clone or download this repository

### 2. Hardware Configuration
If using different pins, update the pin definitions in the header files:
```cpp
// In include/led-controller.h
#define LED_PIN 2

// In include/ir-controller.h  
#define IR_SENSOR_GOAL_1_PIN 18
#define IR_SENSOR_GOAL_2_PIN 19
```

### 3. Upload Code
1. Connect your ESP32 to your computer
2. Open the project in PlatformIO
3. Build and upload: `pio run --target upload`
4. Open Serial Monitor: `pio device monitor` (baud rate: 9600)

## 🎮 Usage

### Normal Operation
1. Power on the system
2. LED strip will show white light initially
3. Demo effects cycle every 10 seconds when not playing

### Playing Soccer
1. Start playing - the system is always ready
2. When a goal is scored, the IR sensor detects it immediately
3. LED strip shows a 3-second celebration in team colors:
   - **Team A Goal**: Red wave celebration
   - **Team B Goal**: Blue wave celebration
4. After celebration, demo effects resume

### Serial Monitor Output
```
ESP32 Soccer Table - WiFi Setup
================================
LED strip initialized with 300 LEDs
IR sensors initialized:
- Goal 1 (Team A) sensor on pin 18
- Goal 2 (Team B) sensor on pin 19
⚽ Soccer table system initialized successfully! ⚽

🥅 GOAL SCORED! 🥅
Team A scored!
Current Score - Team A: 1 | Team B: 0
🎉 Starting goal celebration for Team A (RED)
```

## ⚙️ Configuration Options

### LED Effects
You can modify the available effects in `led-controller.h`:
- `LED_FULL_WHITE` - All sections white
- `LED_COLOR_WAVE` - Colored wave animation  
- `LED_RAINBOW_WAVE` - Rainbow wave effect
- `LED_BREATHING` - Breathing/fade effect

### Timing Settings
Adjust timing in the header files:
```cpp
#define CELEBRATION_DURATION 5000    // Goal celebration length (ms)
#define IR_DEBOUNCE_TIME 500         // Goal detection debounce (ms)
#define WAVE_SPEED 50                // Wave animation speed
```

### Team Colors
Change team colors in `led-controller.h`:
```cpp
#define TEAM_A_COLOR CRGB::Yellow       // Team A color
#define TEAM_B_COLOR CRGB::Orange       // Team B color
```

## 🛠️ Troubleshooting

### LED Strip Issues
- **No LEDs lighting**: Check 5V power supply and data pin connection
- **Wrong colors**: Verify `COLOR_ORDER` in led-controller.h (try RGB vs GRB)
- **Flickering**: Ensure stable 5V power supply with sufficient amperage

### IR Sensor Issues
- **No goal detection**: Check sensor wiring and position
- **False triggers**: Increase `IR_DEBOUNCE_TIME` or adjust sensor sensitivity
- **Sensors always triggered**: Check pull-up resistors and sensor polarity

### Serial Monitor
- Use 9600 baud rate
- Check USB cable and driver installation
- Verify correct COM port selection

## 🔧 Advanced Features

### Add More Effects
Create custom LED effects by adding new cases to the `updateLEDs()` function in `led-controller.cpp`.

## 📊 Power Consumption

- **LED Strip**: Up to 8 at full white (300 LEDs)
- **IR Sensors**: ~20mA each

**Recommended**: 5V 10A power supply for full brightness operation.

## 🤝 Contributing

Feel free to contribute improvements:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📄 License

This project is open source. Feel free to use, modify, and distribute.

## 🎯 Future Enhancements

- [ ] Web dashboard for remote monitoring
- [ ] Sound effects on goals
- [ ] Tournament mode with multiple games
- [ ] Mobile app integration
- [ ] Statistics tracking and analytics
- [ ] Multiple game modes

---

**Made with ⚽ for soccer/foosball enthusiasts!**
*Enjoy your smart soccer table and may the best team win!* 🏆
