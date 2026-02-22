//TV-B-GONE FOR ESP32 DEVKITV1
//Made by Hoang Xuan Bach
//Discord:bch4x.lc96
//Github:xuanbachkat13

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// --- Configuration ---
const uint16_t kIrLedPin   = 4; 
const uint16_t kSwitchPin  = 18; 
const uint16_t kBootButton = 0; 
const uint16_t kStatusLed  = 2;  

IRsend irsend(kIrLedPin);

// --- Data Structure ---
enum Region { REGION_EA, REGION_EU };
struct IRCode {
    decode_type_t protocol;
    uint64_t address;
    uint16_t nbits;
    const char* name;
};

// Danh sách mã Power thực tế (Có thể mở rộng thêm tại đây)
const IRCode eaCodes[] = {
    {SONY, 0xa90, 12, "Sony"},
    {PANASONIC, 0x40040100BCBD, 48, "Panasonic"},
    {NEC, 0x02FD48B7, 32, "NEC"}
};

const IRCode euCodes[] = {
    {RC6, 0xc, 20, "RC6"},
    {LG, 0x20DF10EF, 32, "LG"},
    {SAMSUNG, 0xE0E040BF, 32, "Samsung"}
};

// --- State Management ---
bool isRunning = false;
int currentIndex = 0;
unsigned long lastSendTime = 0;
const int intervalBetweenCodes = 200; // ms

void setup() {
    Serial.begin(115200);
    irsend.begin();
    pinMode(kSwitchPin, INPUT_PULLUP);
    pinMode(kBootButton, INPUT_PULLUP);
    pinMode(kStatusLed, OUTPUT);
    
    Serial.println("--- TV-B-GONE PRO READY ---");
}

void sendCode(IRCode code) {
    digitalWrite(kStatusLed, HIGH);
    Serial.printf("Sending [%s] using Protocol %d\n", code.name, code.protocol);
    
    // Gửi lệnh IR theo giao thức định nghĩa
    irsend.send(code.protocol, code.address, code.nbits);
    
    delay(50); // Flash LED ngắn để nhận biết
    digitalWrite(kStatusLed, LOW);
}

void loop() {
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(kBootButton);

    // Xử lý nút bấm (Toggle)
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        isRunning = !isRunning;
        currentIndex = 0;
        Serial.printf("System %s\n", isRunning ? "STARTED" : "STOPPED");
        delay(300); // Debounce
    }
    lastButtonState = currentButtonState;

    if (isRunning) {
        unsigned long now = millis();
        if (now - lastSendTime >= intervalBetweenCodes) {
            lastSendTime = now;

            Region currentRegion = (digitalRead(kSwitchPin) == LOW) ? REGION_EA : REGION_EU;
            
            if (currentRegion == REGION_EA) {
                if (currentIndex < sizeof(eaCodes) / sizeof(IRCode)) {
                    sendCode(eaCodes[currentIndex++]);
                } else {
                    isRunning = false; // Chạy hết danh sách thì dừng
                    Serial.println("EA Sequence Finished.");
                }
            } else {
                if (currentIndex < sizeof(euCodes) / sizeof(IRCode)) {
                    sendCode(euCodes[currentIndex++]);
                } else {
                    isRunning = false;
                    Serial.println("EU Sequence Finished.");
                }
            }
        }
    }
}
