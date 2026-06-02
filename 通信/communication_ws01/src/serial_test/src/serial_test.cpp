
#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <vector>

int main(int argc, char** argv)
{
    int data[1024];
    ros::init(argc, argv, "serial_port");
    
    ros::NodeHandle n;
    
    
    serial::Serial sp;
    
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    
    sp.setPort("/dev/ttyUSB0");
    
    sp.setBaudrate(115200);
    
    sp.setTimeout(to);
 
    try
    {
        
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }

    
    ros::Publisher pub1 = n.advertise<std_msgs::Int32MultiArray>("mode_data", 1000);
    ros::Publisher pub2 = n.advertise<std_msgs::Int32MultiArray>("DATA", 1000);
    ros::Rate loop_rate(500);

    while(ros::ok())
    {
        size_t n = sp.available();
        if(n != 0)
        {
            uint8_t buffer[1024];
            n = sp.read(buffer, n);
            
            
            ROS_INFO("Received raw data:");
            for(size_t i = 0; i < n; ++i)
            {
                printf("%02X ", buffer[i]);
            }
            printf("\n");

            bool found_header = false;
            bool found_footer = false;
            bool found_header1 = false;
            bool found_footer1 = false;
            std::vector<int> parsed_data;
            std::vector<int> parsed_data1;

            for(size_t i = 0; i < n; ++i)
            {
                if(buffer[i] == 0x7b) 
                {
                    found_header = true;
                    found_footer = false;
                    parsed_data.clear(); 
                }
                else if(buffer[i] == 0x7d && found_header) 
                {
                    found_footer = true;
                }
                else if(found_header && !found_footer)
                {
                    parsed_data.push_back(buffer[i]);
                }

                if(buffer[i] == 0xFF) 
                {
                    found_header1 = true;
                    found_footer1 = false;
                    parsed_data1.clear(); 
                }
                else if(buffer[i] == 0xFE && found_header1) 
                {
                    found_footer1 = true;
                }
                else if(found_header1 && !found_footer1)
                {
                    parsed_data1.push_back(buffer[i]);
                }
            }

            if(found_header && found_footer && !parsed_data.empty())
            {
                std_msgs::Int32MultiArray msg;
                msg.data.clear();
                
                
                int valid_data1 = parsed_data[0];
                msg.data.push_back(valid_data1);

                ROS_INFO("Received mode data: %x", valid_data1);
                pub1.publish(msg);
            }
            else
            {
                ROS_WARN("Header and footer not found or parsed data is empty for mode data");
            }

            if(found_header1 && found_footer1 && !parsed_data1.empty())
            {
                std_msgs::Int32MultiArray msg1;
                msg1.data.clear();
                
                
                int valid_data2 = parsed_data1[0];
                msg1.data.push_back(valid_data2);

                ROS_INFO("Received DATA data: %x", valid_data2);
                pub2.publish(msg1);
            }
            else
            {
                ROS_WARN("Header and footer not found or parsed data is empty for DATA");
            }
        }

        loop_rate.sleep();
    }
    
    sp.close();
 
    return 0;
}
