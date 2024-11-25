#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

const char* ssid = "Mutiara-Wrt";
const char* password = "12345678";

const char* apiUrl = "http://192.168.1.1/api/network/status";

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(115200);
    lcd.begin();
    lcd.backlight();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        lcd.setCursor(0, 0);
        lcd.print("Connecting...");
    }
    lcd.clear();
    lcd.print("WiFi Connected");
    delay(1000);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(apiUrl);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println(payload);
            parseData(payload);
        } else {
            lcd.clear();
            lcd.print("Error fetching");
            lcd.setCursor(0, 1);
            lcd.print("Data");
        }
        http.end();
    }
    delay(5000);
}

void parseData(String payload) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);

    const char* downloadSpeed = doc["download_speed"];
    const char* uploadSpeed = doc["upload_speed"];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Down: ");
    lcd.print(downloadSpeed);

    lcd.setCursor(0, 1);
    lcd.print("Up: ");
    lcd.print(uploadSpeed);
}
