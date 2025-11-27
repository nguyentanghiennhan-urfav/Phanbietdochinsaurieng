#include <WiFi.h>
#include <HTTPClient.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// ====== CẤU HÌNH LCD ======
LiquidCrystal lcd(, 16, 2);

// ====== CẤU HÌNH SERVO ======
Servo servo;
int servoPin = 9;

// ====== CẢM BIẾN ÂM THANH ======
#define SOUND_PIN A2

// ====== WIFI ======
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== URL SERVER AI ======
String serverURL = "https://YOUR_SERVER_URL/predict";

// =========================================
// ============  SETUP  ====================
// =========================================
void setup() {
  Serial.begin(115200);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bắt đầu thu âm sầu riêng thôi nhé!");

  // Servo
  servo.attach(servoPin);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  lcd.setCursor(0, 1);
  lcd.print("WiFi đã kết nối");
  delay(1000);
  lcd.clear();
}

// =========================================
// ============ HÀM GÕ SẦU RIÊNG ============
// =========================================
void knockDurian() {
  servo.write(0);
  delay(20);
  servo.write(150);
  delay(20);
  servo.write(0);
  delay(20);
}

// =========================================
// === Gửi dữ liệu âm thanh lên CLOUD ======
// =========================================
String sendToAI(int soundValue) {
  if (WiFi.status() != WL_CONNECTED) return "wifi_error";

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  // JSON gửi lên server
  String jsonData = "{\"sound\":" + String(soundValue) + "}";

  int httpCode = http.POST(jsonData);
  if (httpCode <= 0) return "http_error";

  String payload = http.getString();
  http.end();
  return payload;
}

// =========================================
// ================= LOOP ==================
// =========================================
void loop() {

  // 1. Gõ sầu riêng
  knockDurian();

  // 2. Đọc âm thanh
  int soundValue = analogRead(SOUND_PIN);
  Serial.print("Sound = ");
  Serial.println(soundValue);

  // 3. Gửi lên AI
  String result = sendToAI(soundValue);
  Serial.println("AI: " + result);

  // 4. Hiển thị LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ket qua AI:");

  lcd.setCursor(0, 1);

  if (result.indexOf("Chin") >= 0) {
    lcd.print("-> Sầu riêng đã chín. Hãy thu hoạch nhé");
  }
  else if (result.indexOf("Gan chin") >= 0) {
    lcd.print("-> Gần chín. Thu hoạch sau nhé");
  }
  else if (result.indexOf("Chua chin") >= 0) {
    lcd.print("-> Chưa chín. Thu hoạch sau nhé");
  } 
   else if (result.indexOf("Nhieu nen") >= 0) {
    lcd.print("-> Âm thanh nhiễu");
  else {
    lcd.print("-> Lỗi AI. Hãy thử lại");
  }

  delay(1500); // chờ trước khi gõ lần tiếp
}

