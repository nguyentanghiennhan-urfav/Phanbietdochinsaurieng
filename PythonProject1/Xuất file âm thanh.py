import serial
import numpy as np
from scipy.io.wavfile import write

PORT = "COM5"         # ⚠️ đổi theo máy bạn
BAUD = 115200
SAMPLE_RATE = 8000

ser = serial.Serial(PORT, BAUD)

print("📡 Đang chờ tín hiệu START...")

# đợi ESP32 gửi START
while True:
    line = ser.readline().decode(errors="ignore").strip()
    if line == "START":
        print("🎤 Bắt đầu thu âm...")
        break

audio_bytes = bytearray()

# thu cho tới khi nhận END
while True:
    if ser.in_waiting:
        chunk = ser.read(ser.in_waiting)
        audio_bytes.extend(chunk)

        if b"END" in audio_bytes:
            idx = audio_bytes.index(b"END")
            audio_bytes = audio_bytes[:idx]   # cắt END
            break

print("💾 Đã nhận dữ liệu, đang chuyển sang WAV...")

# Chuyển byte → mảng int16
audio = np.frombuffer(audio_bytes, dtype=np.int16)

write("sound.wav", SAMPLE_RATE, audio)

print("✅ Hoàn tất!")
print("📁 File đã lưu: sound.wav")