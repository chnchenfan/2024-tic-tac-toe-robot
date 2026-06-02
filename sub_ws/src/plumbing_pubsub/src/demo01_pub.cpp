#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include <sstream>
#include <vector>

int main(int argc, char *argv[]) {
    
    setlocale(LC_ALL, "");

    
    ros::init(argc, argv, "int_data_publisher"); 

    
    ros::NodeHandle nh;

    
    ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("DATA", 10); 

    
    
    std_msgs::Int32MultiArray msg;

    
    ros::Rate rate(10); 

    
    ros::Duration(3).sleep(); 
    
        
        std::vector<int32_t> data(18);

        
        for (int i = 0; i < 18; ++i) {
            data[i] = i * 100; 
        }

        
        msg.data = data;

        
        pub.publish(msg);

        
        ROS_INFO("发布的数据是：");
        for (size_t i = 0; i < msg.data.size(); ++i) {
            ROS_INFO("data[%zu]: %d", i, msg.data[i]);
        }

        
        rate.sleep();
    
    return 0;
}
