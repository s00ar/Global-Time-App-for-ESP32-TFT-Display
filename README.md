# Global Time Display with ESP32 and TFT Screen

**Short description:** A portable ESP32-based clock that shows the current time in Argentina, Italy, and Ecuador with +1/-1 hour references, using a TFT screen and WiFi NTP sync.

This project is a portable global time display built using an ESP32 development board and a TFT LCD screen (ST7789). It shows the current time in three countries (Argentina, Italy, Ecuador), including one hour ahead and one hour behind for each, arranged in clear columns with a clean user interface. Ideal for travel, education, or just keeping in sync with multiple timezones.

---

## 🧰 Technologies Used

### 🔧 Hardware
- **ESP32 Development Board** (Ideaspark or compatible, with CH340 and USB-C)
- **1.9" TFT LCD Display** (ST7789 controller, 170x320 pixels)
- **Portable USB Battery** (Power Bank or LiPo setup)
- **Optional**:
  - USB multiplexer / OTG Adapter
  - Push-button (BOOT button of ESP32)
  - Pendrive or SD Card via adapter for future expansions

### 💻 Software
- **Arduino IDE**
- **TFT_eSPI Library** (for screen control)
- **WiFi Library** (built-in ESP32)
- **time.h** (for NTP time sync)

---

## 💡 Features

- 🌐 Displays current time in three locations:
  - Argentina (UTC-3)
  - Italy (UTC+2 for DST)
  - Ecuador (UTC-5)
- 🕒 For each zone: shows current time, one hour ahead, and one hour behind
- 🖼️ Clean UI with three columns and vertical separators
- 🔘 BOOT button allows simulation of hour changes for visualization
- ⚡ Can run on battery or USB

---

## 🚀 Getting Started

### 📦 Required Libraries
Install from Arduino Library Manager:
- `TFT_eSPI` (by Bodmer)

Configure `User_Setup.h` inside `TFT_eSPI`:
```cpp
#define ST7789_DRIVER
#define TFT_WIDTH 170
#define TFT_HEIGHT 320
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32
#define SPI_FREQUENCY 40000000
```

### 🔌 Wiring Summary
| TFT Pin | ESP32 Pin |
|---------|------------|
| VCC     | 3.3V / 5V  |
| GND     | GND        |
| SCL     | GPIO 18    |
| SDA     | GPIO 23    |
| RES     | GPIO 4     |
| DC      | GPIO 2     |
| CS      | GPIO 15    |
| BL      | GPIO 32    |

---

## 📷 Screenshots
![App UI on screen](preview.jpg) *(Insert screenshot of device running)*

---

## 📄 License
MIT License

---

## 🤓 Credits
Project by [Santiago](https://github.com/s00ar). Special thanks to the open source community.

---

## 📈 Future Improvements
- Add touch input support
- Integrate DST detection dynamically
- Add date and city icons
- Switchable views or scrollable timezone list

---

### Contributions Welcome!
Feel free to fork and improve the code, create issues or submit pull requests 🚀

