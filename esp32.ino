#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Mutiara-Wrt";
const char* password = "12345678";

const char* serverUrl = "http://192.168.1.1/api/network/status";

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
    http.begin(serverUrl);

    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Network Data:");
      lcd.setCursor(0, 1);
      lcd.print(payload);
    } else {
      Serial.println("Error fetching data");
    }

    http.end();
  }

  delay(5000);
}
