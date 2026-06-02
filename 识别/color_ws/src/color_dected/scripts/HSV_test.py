import cv2
import numpy as np


def update_hue_lower(val):
    global lower_hue
    lower_hue = val

def update_hue_upper(val):
    global upper_hue
    upper_hue = val

def update_saturation_lower(val):
    global lower_saturation
    lower_saturation = val

def update_saturation_upper(val):
    global upper_saturation
    upper_saturation = val

def update_value_lower(val):
    global lower_value
    lower_value = val

def update_value_upper(val):
    global upper_value
    upper_value = val


lower_hue = 0
upper_hue = 180
lower_saturation = 0
upper_saturation = 255
lower_value = 0
upper_value = 255


cv2.namedWindow("HSV Threshold Adjustment")


cv2.createTrackbar("Lower Hue", "HSV Threshold Adjustment", lower_hue, 180, update_hue_lower)
cv2.createTrackbar("Upper Hue", "HSV Threshold Adjustment", upper_hue, 180, update_hue_upper)
cv2.createTrackbar("Lower Saturation", "HSV Threshold Adjustment", lower_saturation, 255, update_saturation_lower)
cv2.createTrackbar("Upper Saturation", "HSV Threshold Adjustment", upper_saturation, 255, update_saturation_upper)
cv2.createTrackbar("Lower Value", "HSV Threshold Adjustment", lower_value, 255, update_value_lower)
cv2.createTrackbar("Upper Value", "HSV Threshold Adjustment", upper_value, 255, update_value_upper)


cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("无法打开摄像头")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        break


    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)


    lower_bound = np.array([lower_hue, lower_saturation, lower_value])
    upper_bound = np.array([upper_hue, upper_saturation, upper_value])
    mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)


    result = cv2.bitwise_and(frame, frame, mask=mask)


    cv2.imshow("Mask", mask)
    cv2.imshow("Result", result)
    cv2.imshow("Frame", frame)


    key = cv2.waitKey(1)
    if key == 27:
        break

cap.release()
cv2.destroyAllWindows()
