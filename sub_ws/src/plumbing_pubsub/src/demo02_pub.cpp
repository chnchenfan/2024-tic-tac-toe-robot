#include"ros/ros.h"
#include"std_msgs/String.h"
#include <sstream>
















int main(int argc, char *argv[])
{
   setlocale(LC_ALL,"");
    
    ros::init(argc,argv,"erGouZi");
   
   ros::NodeHandle  nh;
   ros::Rate rate(20.0);
   
          ros::Publisher  pub = nh.advertise<std_msgs::String>("chatter",10); 
   
   
   
    
   int number=3;
std_msgs::String  msg ;
      std::stringstream ss;
      ss  <<"hello --->"<< number;
      msg.data=ss.str();
      for(int i = 10; ros::ok() && i > 0; --i){
        pub.publish(msg);
        ROS_INFO("发布的数据是：%s",ss.str().c_str());
        ros::spinOnce();
        rate.sleep();
	}
 
   
    return  0;
}
