#! /usr/bin/env python3

import cv2
import numpy as np
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String,Float32MultiArray
from cv_bridge import CvBridge, CvBridgeError
import math
























class ColorDetectorNode:
    def __init__(self):
        rospy.loginfo("正在初始化......")
        self.bridge = CvBridge()
        self.image_sub = None
        self.image_pub = None


        self.imgResult = None
        self.pub = None
        self.init_ros_components()

    def init_ros_components(self):
        rospy.init_node("color_detect")
        rospy.loginfo("订阅摄像头......")
        self.image_sub = rospy.Subscriber("/camera/color/image_raw", Image, self.image_callback)

        rospy.loginfo("发布图像数据......")
        self.image_pub = rospy.Publisher("/image_converter/output_video", Image, queue_size=1)
        rospy.loginfo("发布中心点数据......")
        self.centers_pub = rospy.Publisher("/transformed_centers", Float32MultiArray, queue_size=1)

    def image_callback(self,msg):
        rospy.loginfo("成功订阅到摄像头......")
        try:
            img = self.bridge.imgmsg_to_cv2(msg, "bgr8")
            self.imgResult = img.copy()
            rospy.loginfo("img——to-cv2搭建完成......")
            image=self.detect(self.imgResult)
            rospy.loginfo("开始搭建cvbridge.....")
            img_msg = self.bridge.cv2_to_imgmsg(image, encoding="bgr8")
            rospy.loginfo("cvbridge搭建完成.....")
            self.image_pub.publish(img_msg)
            rospy.loginfo("已发布......")
        except CvBridgeError as e:
            rospy.logerr("CvBridge Error: %s", e)

    def find_grid(self,image, corner_points):

        standard_points = np.array([
            [0, 0],
            [3, 0],
            [3, 3],
            [0, 3]
        ], dtype=np.float32)

        M = cv2.getPerspectiveTransform(standard_points, corner_points.astype(np.float32))
        centers = []
        for i in range(3):
            for j in range(3):
                x_center = j + 0.5
                y_center = i + 0.5
                centers.append([x_center, y_center])

        centers = np.array(centers, dtype=np.float32).reshape(-1, 1, 2)
        transformed_centers = cv2.perspectiveTransform(centers, M).reshape(-1, 2)

        centers_list = transformed_centers.flatten().tolist()
        centers_msg = Float32MultiArray(data=centers_list)
        self.centers_pub.publish(centers_msg)
        rospy.loginfo("已发布中心点数据......")
        count = 1
        for point in transformed_centers:
            cv2.circle(image, (int(point[0]), int(point[1])), 5, (0, 0, 255), -1)
            cv2.putText(image, f'{count}', (int(point[0]), int(point[1])), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)
            count += 1


        return image

    def calculate_angle(self,point1_line1, point2_line2, common_point):
        vector1 = np.array([point1_line1[0] - common_point[0], point1_line1[1] - common_point[1]])
        vector2 = np.array([point2_line2[0] - common_point[0], point2_line2[1] - common_point[1]])
        dot_product = np.dot(vector1, vector2)
        magnitude1 = np.linalg.norm(vector1)
        magnitude2 = np.linalg.norm(vector2)
        cos_theta = dot_product / (magnitude1 * magnitude2)
        angle = math.degrees(math.acos(cos_theta))
        return angle


    def detect(self,image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        edges = cv2.Canny(blurred, 50, 150)
        contours, _ = cv2.findContours(edges.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        for contour in contours:
            epsilon = 0.02 * cv2.arcLength(contour, True)
            approx = cv2.approxPolyDP(contour, epsilon, True)
            if len(approx) == 4:

                rect = cv2.minAreaRect(contour)
                box = cv2.boxPoints(rect)
                box = np.int0(box)
                point_list = []
                for point in box:
                    point_list.append(point)

                    cv2.putText(image, f'({point[0]}, {point[1]})', (point[0], point[1]),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5,
                                (0, 255, 0), 1)











                angle1 = self.calculate_angle((point_list[1][0], point_list[1][1]),
                                        (point_list[0][0] + 100, point_list[0][1]),
                                        (point_list[0][0], point_list[0][1]))

                angle2 = 90 - angle1
                new_list = np.array([[point_list[1][0], point_list[1][1]], [point_list[2][0], point_list[2][1]],
                                    [point_list[3][0], point_list[3][1]],
                                    [point_list[0][0], point_list[0][1]]])




                if angle1 < angle2:
                    image = self.find_grid(image, np.array(point_list))

                elif angle1 > angle2:
                    image = self.find_grid(image, new_list)
                elif angle1 == 0 and angle2 == 90:
                    image = self.find_grid(image, np.array(point_list))

                cv2.drawContours(image, [box], 0, (0, 0, 255), 2)
            else:
                cv2.drawContours(image, [contour], -1, (0, 255, 0), 2)
            M = cv2.moments(contour)
            if M['m00'] != 0:
                cx = int(M['m10'] / M['m00'])
                cy = int(M['m01'] / M['m00'])

                cv2.circle(image, (cx, cy), 5, (0, 0, 255), -1)
                cv2.putText(image, f'({cx}, {cy})', (cx + 10, cy - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5,
                            (0, 0, 255), 1)
        return image

if __name__ == "__main__":
    color_detector_node = ColorDetectorNode()
    rospy.spin()
