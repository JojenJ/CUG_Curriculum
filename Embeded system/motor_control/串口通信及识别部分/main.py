import cv2
import torch
import time
import serial

# 串口初始化
ser = serial.Serial('COM3', 9600, timeout=1)  # 替换为正确的COM口号
time.sleep(2)  # 确保串口打开后有足够时间初始化
flag = 0

# 定义物体类别与串口数据的映射
category_to_data = {
    'cat': [0x01],
    'dog': [0x00],
}

# 发送数据（逐字节发送）
def send_hex_data(data):
    for byte in data:
        ser.write(bytes([byte]))  # 逐字节发送
        print(f"Sent: {hex(byte)}")
        time.sleep(0.5)  # 每字节发送间隔，防止接收端丢包

# 加载自定义的 YOLOv5 模型
model_path = 'C:/Users/wwwcz/PycharmProjects/opencv_exam/yolov5/runs/train/exp21/weights/best.pt'  # 替换为你的模型路径
model = torch.hub.load('ultralytics/yolov5', 'custom', path=model_path)  # 加载模型文件
model.eval()

# 打开摄像头
cap = cv2.VideoCapture(0)  # 0表示默认摄像头

# 设置摄像头窗口
cap.set(3, 640)  # 设置宽度
cap.set(4, 640)  # 设置高度
cap.set(cv2.CAP_PROP_EXPOSURE, -5)  # 曝光度调节：较低的曝光值
# 控制帧率：设置每隔多少帧才进行一次推理
frame_skip = 2  # 每隔2帧处理一次
frame_count = 0

# 检测超时（超过2秒发送串口数据）
detection_time = 0
last_detection_time = 0
detection_triggered = False

while True:
    # 捕捉一帧图像
    ret, frame = cap.read()
    if not ret:
        print("无法读取摄像头数据")
        break

    frame_count += 1

    # 仅每隔 frame_skip 帧才进行推理
    if frame_count % frame_skip != 0:
        continue

    # 调整图像大小
    frame_resized = cv2.resize(frame, (640, 640))  # 调整为640x640
    img = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)  # 转换为RGB格式

    # 使用模型直接进行推理
    results = model(img)

    # 获取渲染后的图像
    processed_img = results.ims[0]  # 获取第一张处理后的图像
    processed_img = cv2.cvtColor(processed_img, cv2.COLOR_RGB2BGR)

    # 显示处理后的图像
    cv2.imshow("YOLOv5 Object Detection", processed_img)

    # 获取检测结果
    detections = results.pandas().xywh[0]

    # 检查是否有猫或狗物体被检测到，并且根据物体类别发送对应的数据
    for _, detection in detections.iterrows():
        label = detection['name']  # 获取物体类别名
        confidence = detection['confidence']  # 获取置信度

        if label in category_to_data and confidence > 0.7:  # 只处理猫和狗，置信度大于0.3
            # 根据物体类别发送对应的串口数据
            send_hex_data(category_to_data[label])
            print(f"Detected {label} with confidence {confidence:.2f}")
            detection_triggered = True  # 标记为已触发检测
            last_detection_time = time.time()  # 更新检测时间

    # 如果检测超过2秒，发送串口数据
    if detection_triggered and (time.time() - last_detection_time) > 2 and flag == 0:
        flag = 1
        detection_triggered = False  # 重置检测状态

    # 按 'q' 键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭窗口
cap.release()
cv2.destroyAllWindows()

# 关闭串口
ser.close()
