#ifndef MBOT_LINUX_SERIAL_H
#define MBOT_LINUX_SERIAL_H

#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>
#include <geometry_msgs/Twist.h>






extern void serialInit(boost::asio::serial_port& sp);
extern void writeSpeed1(boost::asio::serial_port& sp, int data1,int data2,int data3);
extern void writeSpeed2(boost::asio::serial_port& sp);
#endif
