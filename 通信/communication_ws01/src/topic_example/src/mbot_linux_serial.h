#ifndef MBOT_LINUX_SERIAL_H
#define MBOT_LINUX_SERIAL_H

#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>
#include <geometry_msgs/Twist.h>
#include <vector>


extern void serialInit(boost::asio::serial_port& sp);


extern void writeSpeed2(boost::asio::serial_port& sp, const std::vector<int>& data);
extern void writeSpeed3(boost::asio::serial_port& sp, const std::vector<int>& data);
extern void writeSpeed4(boost::asio::serial_port& sp, const std::vector<int>& data);
#endif 
