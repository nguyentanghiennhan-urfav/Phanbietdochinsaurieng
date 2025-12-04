#define MIC_PIN D11
#define SAMPLE_RATE 8000
#define RECORD_SECONDS 2
#define NUM_SAMPLES (SAMPLE_RATE * RECORD_SECONDS)

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("START");   // báo PC bắt đầu nhận
}

void loop() {
    // Gửi 2 giây âm thanh mỗi lần nhấn reset
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw = analogRead(MIC_PIN);  // 0–4095
        int16_t sample = (raw - 2048) * 16; // chuẩn hóa về 16-bit PCM
        
        Serial.write((uint8_t*)&sample, 2);
        delayMicroseconds(1000000 / SAMPLE_RATE);
    }

    Serial.println("END");   // báo hiệu PC dừng ghi
    while (true);            // dừng 
}