#include "ros/ros.h"
#include "std_msgs/String.h" 
#include "mbot_linux_serial.h"
boost::asio::io_service iosev1, iosev2;
boost::asio::serial_port sp1(iosev1, "/dev/ttyUSB0");












void doMsg(const std_msgs::String::ConstPtr& msg_p){
    if(msg_p->data == "hello --->4"){
        ROS_INFO("first");
        writeSpeed1(sp1, 0x01,2,3);
        
    }
    else if (msg_p->data == "hello --->5")
    {
        ROS_INFO("second");
        
    }
}

int main(int agrc,char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(agrc,argv,"public_node");
    ros::NodeHandle nh;
    ROS_INFO("arm stary....");
    
    try { 
        serialInit(sp1); 
        
        } 
    catch (boost::system::system_error& e)
     { 
        ROS_ERROR("Error initializing serial ports: %s", e.what()); 
        return -1; 
    }
     writeSpeed1(sp1, 0x01,2,3);







   



    
    ros::Duration(2).sleep();
    
    ros::spin();
    return 0;
}
 

