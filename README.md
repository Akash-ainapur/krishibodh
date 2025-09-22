# FarmBot Control System - KrishiBodh

A comprehensive plant-watering automation system that combines Python backend, web frontend, and Arduino-based hardware control for precise plant care.

## 🌱 Project Overview

KrishiBodh is an intelligent plant watering system that uses:

- **AI-powered command processing** via Google Gemini API
- **Web-based control interface** with real-time status updates
- **Arduino-controlled stepper motors** for precise positioning
- **Soil moisture sensing** for smart watering decisions
- **Automated water pump control** based on plant needs

## 📁 Project Structure

```
Working/
├── app.py                           # Flask backend with AI integration
├── index.html                       # Web control interface
├── sketches/                        # Arduino firmware
│   ├── with_moisture_and_z_back.ino # Advanced version with moisture sensing
│   └── x_y_z_water_pump.ino         # Basic version with pump control
└── README.md                        # This file
```

## 🚀 Features

### Web Interface (`index.html`)

- **Real-time Arduino communication** via Web Serial API
- **Predefined plant locations** with one-click navigation
- **AI command processing** for natural language control
- **Status monitoring** with timestamped updates
- **Connection management** for Arduino devices

### Backend API (`app.py`)

- **Flask web server** serving the control interface
- **Google Gemini AI integration** for natural language processing
- **JSON API endpoint** (`/api/ask`) for AI command interpretation
- **Coordinate mapping** for plant locations and home position

### Arduino Firmware

#### Basic Version (`x_y_z_water_pump.ino`)

- **4-axis stepper control** (X, Y, Z, A motors)
- **CNC Shield compatibility** with standard pin mappings
- **Coordinate-based movement** with relative positioning
- **Automatic water pump activation** (5-second duration)
- **Home position detection** to skip watering at origin

#### Advanced Version (`with_moisture_and_z_back.ino`)

- All features from basic version
- **Soil moisture sensing** via analog sensor (A1)
- **Intelligent watering decisions** based on moisture levels
- **Automatic Z-axis return** to home position after watering
- **Configurable moisture threshold** (default: 500)

## 🛠️ Hardware Requirements

### Arduino Setup

- **Arduino Uno/Nano** or compatible microcontroller
- **CNC Shield** for stepper motor control
- **4x Stepper Motors** (NEMA 17 or similar)
- **Stepper Motor Drivers** (A4988 or DRV8825)
- **Relay Module** for water pump control
- **Soil Moisture Sensor** (analog version for advanced firmware)
- **Water Pump** with appropriate tubing

### Pin Connections (CNC Shield Standard)

```
Stepper Motors:
- X-axis: Step=2, Dir=5
- Y-axis: Step=3, Dir=6
- Z-axis: Step=4, Dir=7
- A-axis: Step=12, Dir=13

Additional:
- Relay: Pin 10
- Soil Sensor: A1 (advanced version only)
```

## 📋 Software Requirements

### Python Dependencies

```bash
pip install flask google-generativeai
```

### API Configuration

1. Obtain Google Gemini API key
2. Update `API_KEY` variable in `app.py`
3. Ensure internet connectivity for AI processing

### Browser Compatibility

- **Chrome/Chromium** (required for Web Serial API)
- **Edge** (Web Serial API support)
- **Firefox/Safari**: Not supported (no Web Serial API)

## 🚀 Getting Started

### 1. Arduino Setup

1. Choose appropriate firmware:
   - `x_y_z_water_pump.ino` for basic operation
   - `with_moisture_and_z_back.ino` for advanced features
2. Upload to Arduino with CNC Shield connected
3. Connect stepper motors and water pump relay
4. Open Serial Monitor to verify communication (9600 baud)

### 2. Backend Setup

```bash
# Install dependencies
pip install flask google-generativeai

# Run the Flask server
python app.py
```

Server will start on `http://localhost:5000`

### 3. Web Interface

1. Open browser and navigate to `http://localhost:5000`
2. Click "🔌 Connect" to establish Arduino connection
3. Select COM port when prompted
4. Use plant buttons or AI commands to control the system

## 🎯 Usage Examples

### Manual Control

- **🌱 Plant 1**: Move to coordinates (800, 800, 25000) and water
- **🌱 Plant 2**: Move to coordinates (2400, 1200, 25000) and water
- **🏠 Home**: Return to origin (0, 0, 0)

### AI Commands

Natural language commands that get processed by Gemini AI:

- "Go to plant 1"
- "Water the second plant"
- "Return home"
- "Move to plant 2 and water it"

## ⚙️ Configuration

### Plant Coordinates

Edit coordinates in `app.py` SYSTEM_INSTRUCTION:

```python
- "plant1": {"x": 800, "y": 800, "z": 25000}
- "plant2": {"x": 2400, "y": 1200, "z": 25000}
- "home": {"x": 0, "y": 0, "z": 0}
```

### Moisture Threshold

Adjust in `with_moisture_and_z_back.ino`:

```cpp
const int moistureThreshold = 500; // Below this = dry
```

### Movement Timing

Modify step delays for different speeds:

```cpp
delayMicroseconds(1500); // X/A axis speed
delayMicroseconds(1000); // Y axis speed
delayMicroseconds(500);  // Z axis speed
```

## 🔧 Troubleshooting

### Common Issues

1. **Connection Failed**: Ensure Arduino is connected and firmware is uploaded
2. **Motors Not Moving**: Check stepper driver connections and power supply
3. **AI Commands Not Working**: Verify API key and internet connection
4. **Web Interface Not Loading**: Ensure Flask server is running on port 5000

### Serial Communication

- Monitor Arduino Serial output for status messages
- All status messages are prefixed with "STATUS:" for easy filtering
- Default baud rate: 9600

## 🔒 Security Notes

- **API Key**: The current implementation includes a hardcoded API key in `app.py`
- **Production**: Remove debug mode and implement proper API key management
- **Network**: Consider local network security for production deployments

## 🤝 Contributing

This project demonstrates a complete IoT plant automation system. Key areas for enhancement:

- Enhanced error handling and recovery
- User authentication and access control
- Mobile-responsive web interface
- Data logging and analytics
- Multi-plant scheduling system

## 📄 License

This project is provided as-is for educational and development purposes.

---

**KrishiBodh** - Smart Plant Care Through Technology 🌱🤖
