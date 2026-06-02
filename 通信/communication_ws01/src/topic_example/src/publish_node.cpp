#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h" 
#include "mbot_linux_serial.h"
#include <boost/asio.hpp>
#include <vector>
#include <std_msgs/Int32MultiArray.h>
#include <array>

const int DATA_SIZE = 18;
const int MAX_COUNT = 3;

std::array<std::array<int, DATA_SIZE>, MAX_COUNT> data3 = {0};
std::array<std::array<int, DATA_SIZE>, MAX_COUNT> data4 = {0};
std::array<std::array<int, DATA_SIZE>, MAX_COUNT> data5 = {0};
int mode = 0;
int data_state = 0;
boost::asio::io_service io_service;
boost::asio::serial_port sp(io_service, "/dev/ttyUSB0");

void process_data(const std::array<std::array<int, DATA_SIZE>, MAX_COUNT>& data_array, int count) {
    std::vector<int> averaged_data(DATA_SIZE, 0);
    for (int i = 0; i < DATA_SIZE; ++i) {
        averaged_data[i] = (data_array[0][i] + data_array[1][i] + data_array[2][i]) / MAX_COUNT;
    }
    writeSpeed2(sp, averaged_data);
}


void mode_store(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    mode = msg_p->data[0];
}
void data_store(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    
    data_state = msg_p->data[0]; 
    ROS_INFO("recive data_state: %d",data_state);
    if (data_state == 85)
    {
        data_state = 5;
        ROS_INFO("data_state is : %d",data_state);
    }
}



void chessboard_centers(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    if (mode == 3 || mode == 4 || mode == 5) {
        if(mode == 3){
            ROS_INFO("模式3启动！");
        }else if (mode == 4){
            ROS_INFO("模式4启动！");
        }else if( mode == 5){
            ROS_INFO("模式5启动！");
        }
        
        if (msg_p->data.size() == DATA_SIZE) {
            static int count = 0;  
            if (count < MAX_COUNT) {
                for (size_t i = 0; i < msg_p->data.size(); ++i) {
                    data3[count][i] = static_cast<int>(msg_p->data[i]);
                }
                count++;
            } else {
                ROS_WARN("const3 has exceeded the maximum value");
                process_data(data3, count);
                count = 0;  
                mode = 0;   
            }
        } else {
            ROS_WARN("Received data size is not %d", DATA_SIZE);
        }
    }  
}
void chessboard_states(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    
    
    
    
    
    
    
    
    
    
    
    
            
    
    
}

void target_point_t4(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    if (data_state == 68) {
        ROS_INFO("target_point_t4 is beginning ............");
        std::vector<int> data4_new(DATA_SIZE, 0);
        if (msg_p->data.size() == DATA_SIZE) {
            for (size_t i = 0; i < msg_p->data.size(); ++i) {
                data4_new[i] = static_cast<int>(msg_p->data[i]);
            }
            
            writeSpeed4(sp, data4_new);
            data_state = 0;   
            
            
            
            
            
            
            
            
            
            
            
            
        } else {
            ROS_WARN("Received data size is not %d", DATA_SIZE);
        }
    }
}
void target_point_t5(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    if (data_state == 5) {
        ROS_INFO("target_point_t5 is beginning ............");
        std::vector<int> data5_new(DATA_SIZE, 0);
        if (msg_p->data.size() == DATA_SIZE) {
            for (size_t i = 0; i < msg_p->data.size(); ++i) {
                data5_new[i] = static_cast<int>(msg_p->data[i]);
            }
            
            writeSpeed4(sp, data5_new);
            data_state = 0;   
            
            
            
            
            
            
            
            
            
            
            
            
        } else {
            ROS_WARN("Received data size is not %d", DATA_SIZE);
        }
    }
}
void target_point_t6(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    if (data_state == 6) {
        ROS_INFO("target_point_t6 is beginning ............");
        std::vector<int> data5_new(DATA_SIZE, 0);
        if (msg_p->data.size() == DATA_SIZE) {
            for (size_t i = 0; i < msg_p->data.size(); ++i) {
                data5_new[i] = static_cast<int>(msg_p->data[i]);
            }
            
            writeSpeed4(sp, data5_new);
            data_state = 0;   
            
            
            
            
            
            
            
            
            
            
            
            
        } else {
            ROS_WARN("Received data size is not %d", DATA_SIZE);
        }
    }
}


void mubiaopoint(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    
    
    
    
    
    
    
}
void test(const std_msgs::Int32MultiArray::ConstPtr& msg) {
    ROS_INFO("I heard: ");
    for (size_t i = 0; i < msg->data.size(); ++i) {
        ROS_INFO("data[%zu]: %d", i, msg->data[i]);
    }
    std::vector<int> data(msg->data.begin(), msg->data.end());
    writeSpeed2(sp, data);
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "public_node");
    ros::NodeHandle nh;
    ROS_INFO("数据通信设备准备就绪");

    
    try {
        serialInit(sp);
        ROS_INFO("Serial is ready!");
    } catch (boost::system::system_error& e) {
        ROS_ERROR("Error initializing serial ports: %s", e.what());
        return -1;
    }

    ros::Subscriber sub_mode = nh.subscribe("mode_data", 5, mode_store);
    ros::Subscriber sub_DATA = nh.subscribe("DATA", 40, data_store);

    ros::Subscriber sub_centers = nh.subscribe("/chessboard_centers", 40, chessboard_centers);
    ros::Subscriber sub_states = nh.subscribe("/chessboard_states", 40, chessboard_states);

    ros::Subscriber sub_target_t4 = nh.subscribe("/target_point", 22, target_point_t4);    
    ros::Subscriber sub_target_t5 = nh.subscribe("/target_point_t5", 22, target_point_t5);
    ros::Subscriber sub_target_t6 = nh.subscribe("/target_point_t6", 22, target_point_t6);

    ros::Subscriber sub3 = nh.subscribe("/mubiaopoint", 22, mubiaopoint);
    ros::Subscriber sub_test = nh.subscribe("/Serial_test", 22, test);


    ros::spin();

    return 0;
}
