from flask import Flask, request, jsonify
import tensorflow as tf
import numpy as np

# Tải model AI đã huấn luyện từ Teachable Machine
model = tf.keras.models.load_model("model.h5")

# Tạo server Flask
app = Flask(__name__)

# API nhận dữ liệu âm thanh
@app.route("/predict", methods=["POST"])
def predict():
    # Nhận dữ liệu âm thanh dạng mảng số do Arduino gửi lên
    data = request.json["audio"]
    data = np.array(data).reshape(1, -1)  # Chuyển thành dạng Tensor

    # Cho dữ liệu vào model AI để dự đoán
    prediction = model.predict(data)
    label_index = np.argmax(prediction)

    # Gán tên nhãn theo index
    labels = ["Chín", "Chín trong ngày", "Chưa chín", "Nhiễu nền"]
    result = labels[label_index]

    # Trả về JSON cho Arduino
    return jsonify({"result": result})

# Chạy server
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
