#!/usr/bin/env python3


import rospy
import time
import threading
from Arm_Lib import Arm_Device
from plumbing_sub_pub.msg import pose

class ControlSub:
    def __init__(self):
        self.b_time = 1
        self.arm = Arm_Device()
        time.sleep(0.1)
        self.p = 0
        self.y = 0
        self.r = 0
        self.kp = 0.02
        self.ki = 0
        self.kd = 0
        self.nowerror = 0
        self.lasterror = 0
        self.integral = 0
        self.value_6 = 0
        self.value_sing_6 = 0
        self.count=55
        self.count_flag=0
        self.stop_flag=0
        self.sub = None

    def move_arm(self, hand, attaintion):




        self.p = min(max(self.p, 0), 180)
        self.y = min(max(self.y, 0), 180)
        self.r = min(max(self.r, 0), 180)









        if attaintion == 1:
             self.arm.Arm_serial_servo_write6(self.y, self.p, self.p, self.p, self.r, 20, 500)
        if attaintion == 2:
             self.arm.Arm_serial_servo_write6(self.y, self.p, self.p, self.p, self.r, 150, 500)
        if attaintion == 3:
                self.arm.Arm_serial_servo_write6(90, 102, 110, 110, 90, 100, 500)
                self.y = self.p= self.r =90
        if attaintion == 4:
            if self.count >= 123:
                self.stop_flag= self.stop_flag+1
                self.count_flag = -1
            elif self.count <= 55:
                self.count_flag = 1
            if self.count_flag==-1:
                self.count -= 5
            if self.count_flag==1:
                self.count += 5
            if self.stop_flag <=2:
                self.y = self.p= self.r =90
                self.arm.Arm_serial_servo_write6(self.count, 89, 10, 97, 85, 40, 500)
            elif self.stop_flag >2:
                self.arm.Arm_serial_servo_write6(self.y, self.p, self.p, self.p, self.r, 20, 500)




    def do_msg(self, pose):




















        self.p = pose.pitch + 90
        self.y = pose.yaw + 90
        self.r = pose.roll + 90
        hand = pose.hand
        attaintion = pose.attation


        self.p = min(max(self.p, 0), 180)
        self.y = min(max(self.y, 0), 180)
        self.r = min(max(self.r, 0), 180)


        self.move_arm( hand, attaintion)

    def set_value_6_to_one(self):
        if self.value_sing_6==1:
            time.sleep(5)
            self.value_6 = 1

        time.sleep(5)
        self.value_6 = 2
        time.sleep(5)

        self.value_6 = 3
    def start_subscriber(self):
        rospy.init_node("control_sub")
        self.sub = rospy.Subscriber("arm", pose, self.do_msg, queue_size=5)

        timer_thread = threading.Thread(target=self.set_value_6_to_one)
        timer_thread.start()

        rospy.spin()

if __name__ == "__main__":
    control_sub = ControlSub()
    control_sub.start_subscriber()