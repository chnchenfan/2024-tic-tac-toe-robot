'''
    前三题：发布中心点坐标数据
    四五题：发布棋子状态到算法，算法发布目标点坐标数据

    算法：订阅中心点坐标数据，订阅棋子状态。发布坐标数据
    串口通信：订阅中心点坐标数据，订阅目标点坐标数据

'''
#!/usr/bin/env python3

import cv2
import numpy as np
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import Float32MultiArray,Int32MultiArray,String
from cv_bridge import CvBridge, CvBridgeError
import math


RA = tuple([0, 0])
RB = tuple([0, 0])
RC = tuple([0, 0])
RD = tuple([0, 0])
A = tuple([0, 0])
C = tuple([0, 0])
G = tuple([0, 0])
I = tuple([0, 0])
E = tuple([0, 0])
B = tuple([0, 0])
F = tuple([0, 0])
D = tuple([0, 0])
H = tuple([0, 0])
I = tuple([0, 0])


EMPTY = ' '
BLACK_PIECE = 'X'
WHITE_PIECE = 'O'


def preProcessing(img):

    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    imgBlur = cv2.GaussianBlur(imgGray, (5, 5), 1)
    imgCanny = cv2.Canny(imgBlur, 200, 200)
    kernel = np.ones((5, 5))
    imgDial = cv2.dilate(imgCanny, kernel, iterations=2)
    imgThres = cv2.erode(imgDial, kernel, iterations=1)
    return imgThres


def getContours(img,imgContour):

    global RA, RB, RC, RD, E, A, C, G, I, B, F, D, H

    biggest = np.array([])
    longest = np.array([])
    maxArea = 0
    contours, hierarchy = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    output_image = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

    for cnt in contours:
        area = cv2.contourArea(cnt)
        '''
        遍历所有轮廓。
        计算每个轮廓的面积。
        如果轮廓的面积大于5000：
            计算轮廓的周长。
            使用 cv2.approxPolyDP 对轮廓进行多边形近似。
            如果近似后的多边形有4个顶点，并且它的面积大于当前最大面积：
                更新 biggest 为当前的轮廓。
                更新 maxArea 为当前的面积。
        '''
        if area > 5000:
            peri = cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, 0.02 * peri, True)

            if area > maxArea and len(approx) == 4:
                biggest = approx
                maxArea = area
                longest = cnt
    xx = []
    yy = []
    xx1 = []
    yy1 = []

    if len(biggest) != 0:

        leftmost = sorted([(tuple(point[0]), point[0]) for point in biggest], key=lambda x: x[0][0])[:2]
        RA = min(leftmost, key=lambda x: x[0][1])[0]

        rightmost = sorted([(tuple(point[0]), point[0]) for point in biggest], key=lambda x: x[0][0], reverse=True)[:2]
        RD = max(rightmost, key=lambda x: x[0][1])[0]

        remaining_points = [tuple(point[0]) for point in biggest if tuple(point[0]) not in [RA, RD]]
        RB = min(remaining_points, key=lambda x: x[1])
        RC = [point for point in remaining_points if point != RB][0]

        E = (
            (RA[0] + RB[0] + RC[0] + RD[0]) / 4,
            (RA[1] + RB[1] + RC[1] + RD[1]) / 4
        )


        A = (
            RA[0] + (E[0] - RA[0]) / 3,
            RA[1] + (E[1] - RA[1]) / 3
        )


        C = (
            RB[0] + (E[0] - RB[0]) / 3,
            RB[1] + (E[1] - RB[1]) / 3
        )


        G = (
            RC[0] + (E[0] - RC[0]) / 3,
            RC[1] + (E[1] - RC[1]) / 3
        )


        I = (
            RD[0] + (E[0] - RD[0]) / 3,
            RD[1] + (E[1] - RD[1]) / 3
        )


        B = (
            (A[0] + C[0]) / 2,
            (A[1] + C[1]) / 2
        )


        F = (
            (C[0] + I[0]) / 2,
            (C[1] + I[1]) / 2
        )


        D = (
            (A[0] + G[0]) / 2,
            (A[1] + G[1]) / 2
        )


        H = (
            (G[0] + I[0]) / 2,
            (G[1] + I[1]) / 2
        )


        cv2.circle(imgContour, (int(A[0]), int(A[1])), 5, (126, 126, 0), cv2.FILLED)
        cv2.circle(imgContour, (int(C[0]), int(C[1])), 5, (0, 126, 126), cv2.FILLED)
        cv2.circle(imgContour, (int(G[0]), int(G[1])), 5, (0, 255, 255), cv2.FILLED)
        cv2.circle(imgContour, (int(I[0]), int(I[1])), 5, (0, 255, 255), cv2.FILLED)
        cv2.circle(imgContour, (int(B[0]), int(B[1])), 5, (0, 0, 255), cv2.FILLED)
        cv2.circle(imgContour, (int(F[0]), int(F[1])), 5, (255, 0, 0), cv2.FILLED)
        cv2.circle(imgContour, (int(D[0]), int(D[1])), 5, (255, 255, 0), cv2.FILLED)
        cv2.circle(imgContour, (int(H[0]), int(H[1])), 5, (255, 255, 0), cv2.FILLED)


        cv2.circle(imgContour, (int(E[0]), int(E[1])), 5, (0, 255, 0), cv2.FILLED)

        cv2.drawContours(imgContour, biggest, -1, (255, 0, 0), 20)
        cv2.drawContours(imgContour, longest, -1, (255, 0, 0), 3)

        xx1.append(int(G[0]))
        xx1.append(int(D[0]))
        xx1.append(int(A[0]))
        xx1.append(int(H[0]))
        xx1.append(int(E[0]))
        xx1.append(int(B[0]))
        xx1.append(int(I[0]))
        xx1.append(int(F[0]))
        xx1.append(int(C[0]))

        yy1.append(int(G[1]))
        yy1.append(int(D[1]))
        yy1.append(int(A[1]))
        yy1.append(int(H[1]))
        yy1.append(int(E[1]))
        yy1.append(int(B[1]))
        yy1.append(int(I[1]))
        yy1.append(int(F[1]))
        yy1.append(int(C[1]))

        xx.append(int(A[0]))
        xx.append(int(B[0]))
        xx.append(int(C[0]))
        xx.append(int(D[0]))
        xx.append(int(E[0]))
        xx.append(int(F[0]))
        xx.append(int(G[0]))
        xx.append(int(H[0]))
        xx.append(int(I[0]))

        yy.append(int(A[1]))
        yy.append(int(B[1]))
        yy.append(int(C[1]))
        yy.append(int(D[1]))
        yy.append(int(E[1]))
        yy.append(int(F[1]))
        yy.append(int(G[1]))
        yy.append(int(H[1]))
        yy.append(int(I[1]))
    return xx1,yy1, xx, yy, imgContour

def detect_pieces(img, centers_x, centers_y):
    states = []


    hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)


    lower_black = np.array([0, 0, 0])
    upper_black = np.array([180, 255, 89])
    lower_white = np.array([0,0, 176])
    upper_white = np.array([180, 28, 255])


    board_width = max(RB[0], RC[0]) - min(RA[0], RD[0])
    board_height = max(RC[1], RD[1]) - min(RA[1], RB[1])


    num_cells = 3
    cell_width = board_width // num_cells
    cell_height = board_height // num_cells

    for x, y in zip(centers_x, centers_y):


        x = int(x)
        y = int(y)


        x1 = max(0, x - cell_width // 2)
        y1 = max(0, y - cell_height // 2)
        x2 = min(img.shape[1], x + cell_width // 2)
        y2 = min(img.shape[0], y + cell_height // 2)


        cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)

        region = hsv_img[y1:y2, x1:x2]


        black_mask = cv2.inRange(region, lower_black, upper_black)
        white_mask = cv2.inRange(region, lower_white, upper_white)


        black_area_ratio = np.sum(black_mask == 255) / (region.shape[0] * region.shape[1])
        white_area_ratio = np.sum(white_mask == 255) / (region.shape[0] * region.shape[1])


        center = (x, y)
        radius = min(cell_width, cell_height) // 2

        if black_area_ratio > 0.15:
            states.append(BLACK_PIECE)
            cv2.circle(img, center, radius, (0, 0, 255), 2)
        elif white_area_ratio > 0.1:
            states.append(WHITE_PIECE)
            cv2.circle(img, center, radius, (255, 0, 255), 2)
        else:
            states.append(EMPTY)

    return states,img

class ChessboardDetectorNode:
    def __init__(self):
        self.bridge = CvBridge()
        self.image_sub = None
        self.image_pub = None
        self.centers_pub = None
        self.states_pub = None
        self.imgResult = None
        self.judgedata = 0
        self.init_ros_components()

    def init_ros_components(self):
        rospy.init_node("chessboard_detect")
        rospy.loginfo("订阅摄像头......")
        self.image_sub = rospy.Subscriber("/usb_cam/image_raw", Image, self.image_callback)
        rospy.loginfo("发布图像数据......")
        self.image_pub = rospy.Publisher("/image_converter/output_video", Image, queue_size=1)
        rospy.loginfo("发布中心点数据......")
        self.centers_pub = rospy.Publisher("/chessboard_centers", Int32MultiArray, queue_size=1)
        rospy.loginfo("发布棋子状态数据......")
        self.states_pub = rospy.Publisher("/chessboard_states", String, queue_size=1)

    def image_callback(self, msg):

        try:
            img = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            self.imgResult = img.copy()
            imgContour = self.imgResult.copy()

            imgThres = preProcessing(self.imgResult)

            '''
                imgThres 是经过预处理后的二值图像（例如使用Canny边缘检测后的图像），通常是黑白的，不适合直接绘制彩色的轮廓和标记。
                为了保持原始图像的可读性，以及轮廓和标记的清晰度，通常会在原始图像或者它的副本（即 imgContour）上进行绘制。
            '''
            xx1,yy1, xx, yy,self.imgResult= getContours(imgThres,imgContour)

            states, self.imgResult = detect_pieces(self.imgResult, xx, yy)




            img_msg = self.bridge.cv2_to_imgmsg(self.imgResult, encoding="bgr8")

            self.image_pub.publish(img_msg)




            centers = [int(c) for c in xx1 + yy1]
            centers_msg = Int32MultiArray(data=centers)
            self.centers_pub.publish(centers_msg)




            states_str = ''.join(states)
            states_msg = String(data=states_str)

            self.states_pub.publish(states_msg)
            self.judgedata = 0
            rospy.loginfo("已发布棋子状态数据......")
        except CvBridgeError as e:
            rospy.logerr("CvBridge Error: %s", e)

if __name__ == "__main__":
    chessboard_detector_node = ChessboardDetectorNode()
    rospy.spin()
