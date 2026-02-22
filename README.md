# 📺 ESP32 TV-B-Gone

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Framework: Arduino](https://img.shields.io/badge/Framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![Hardware: ESP32](https://img.shields.io/badge/Hardware-ESP32-E7352C.svg)](https://www.espressif.com/en/products/socs/esp32)

Một thiết bị TV-B-GONE tối giản dựa trên **ESP32 DevKit V1**, có khả năng tắt hầu hết các dòng TV phổ biến (Sony, Samsung, LG, Panasonic, NEC,...) bằng cách quét chuỗi mã Power IR liên tục.

## ✨ Tính năng nổi bật
* ⚡ **Tối ưu hóa phản hồi**: Sử dụng State Machine (Non-blocking) giúp nút bấm phản hồi tức thì, không bị delay bởi lệnh IR.
* 🌍 **Hỗ trợ đa vùng**: Chuyển đổi linh hoạt giữa các bộ mã **EA (Châu Á)** và **EU (Châu Âu)** thông qua Switch gạt.
* 🛠 **Dễ dàng mở rộng**: Cấu trúc dữ liệu dạng `struct` giúp việc thêm mã Hex của các hãng TV mới cực kỳ đơn giản.
* 🚨 **Chỉ báo trực quan**: LED trạng thái (GPIO 2) thông báo khi đang phát tín hiệu.

---

## 🔌 Sơ đồ đấu nối (Hardware)


| Linh kiện | Chân ESP32 (GPIO) | Ghi chú |
| :--- | :--- | :--- |
| **IR LED (Anode)** | Vin (5V) / 3.3V | Cần qua Transistor để phát xa |
| **Transistor Base** | **GPIO 4** | Kích dòng cho IR LED |
| **Slide Switch** | **GPIO 18** | Chọn vùng (GND = EA, Open = EU) |
| **Push Button** | **GPIO 0 (BOOT)** | Nhấn để Bắt đầu/Dừng |
| **Status LED** | **GPIO 2** | LED mặc định trên board |

> **⚠️ Lưu ý:** Để đạt tầm xa trên 5m, bạn nên sử dụng Transistor (ví dụ: 2N2222) thay vì nối trực tiếp LED vào chân GPIO.

---

## 💻 Cài đặt & Sử dụng

### 1. Yêu cầu phần mềm
* **Arduino IDE** hoặc **VS Code + PlatformIO**.
* Thư viện: `IRremoteESP8266` (Cài đặt qua Library Manager).

### 2. Cấu hình Board
* **Board**: `DOIT ESP32 DEVKIT V1`.
* **Flash Frequency**: `80MHz`.
* **Upload Speed**: `921600`.

### 3. Cách sử dụng
1.  Gạt Switch để chọn vùng dữ liệu (EA hoặc EU).
2.  Nhấn nút **BOOT** để bắt đầu quét mã. LED xanh trên board sẽ nhấp nháy.
3.  Nhấn lại nút **BOOT** bất cứ lúc nào để dừng khẩn cấp.

---

## 📂 Cấu trúc mã nguồn
* `struct IRCode`: Định nghĩa giao thức, địa chỉ và số bit.
* `sendCode()`: Hàm xử lý gửi lệnh tổng quát cho mọi giao thức.
* `loop()`: Quản lý trạng thái bằng biến thời gian `millis()`, đảm bảo tính đa nhiệm.

---

## 🤝 Đóng góp (Contribution)
Mọi sự đóng góp về mã Hex của các dòng TV nội địa (Casper, Asanzo,...) đều được hoan nghênh.

* **Author**: Hoang Xuan Bach
* **Discord**: `bch4x.lc96`
* **Github**: [xuanbachkat13](https://github.com/xuanbachkat13)

😤Nếu gặp lỗi,vui lòng tạo đăng lên Issues nha,mình sẽ fix sớm:)))  
✨Và nhớ cho repo này của mình một Stars nhé!
---
*Dự án này được tạo ra cho mục đích học tập và nghiên cứu về giao thức IR*
