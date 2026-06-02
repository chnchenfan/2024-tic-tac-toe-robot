#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <std_msgs/Int32.h>
#include <mutex>
#include <condition_variable>
#include "std_msgs/Int32MultiArray.h" 
#include <boost/asio.hpp>
#include <vector>


struct PoseData {
    uint8_t task;
    uint8_t state;
    uint8_t key1;
    uint8_t key2;
    uint8_t exec;
    uint8_t judge;
};

int received_data = -1;

std::mutex mtx;
std::condition_variable cv;


const int ARRAY_SIZE = 18;
int transformed_centers_data[ARRAY_SIZE];

void doMsg1(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    
     ROS_INFO("已订阅到 中心点 /chessboard_centers 话题");
    if (msg_p->data.size() == ARRAY_SIZE) {
        
        for (size_t i = 0; i < msg_p->data.size(); ++i) {
            transformed_centers_data[i] = static_cast<int>(msg_p->data[i]);
            ROS_INFO("transformed_centers_data[%zu]=%d",i,transformed_centers_data[i] );
        }
        ROS_INFO("Data stored successfully");
    } else {
        ROS_WARN("Received data size is not %d", ARRAY_SIZE);
    }
}

void doMsg2(const std_msgs::Int32MultiArray::ConstPtr& msg_p) {
    
     ROS_INFO("已订阅到 中心点 /chessboard_centers 话题");
    if (msg_p->data.size() == ARRAY_SIZE) {
        
        for (size_t i = 0; i < msg_p->data.size(); ++i) {
            transformed_centers_data[i] = static_cast<int>(msg_p->data[i]);
            ROS_INFO("transformed_centers_data[%zu]=%d",i,transformed_centers_data[i] );
        }
        ROS_INFO("Data stored successfully");
    } else {
        ROS_WARN("Received data size is not %d", ARRAY_SIZE);
    }
}


void Task1(PoseData& pose, serial::Serial& sp);
void Task2(PoseData& pose, serial::Serial& sp);
void Task3(PoseData& pose, serial::Serial& sp);
void Task4(PoseData& pose, serial::Serial& sp,ros::Publisher& pub);
void Task5(PoseData& pose, serial::Serial& sp,ros::Publisher& pub);
void Task6(PoseData& pose, serial::Serial& sp);

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "serial_pub");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32>("control", 5);
    ROS_INFO("开始订阅 中心点 /chessboard_centers 话题");
    ros::Subscriber sub1 = nh.subscribe("/chessboard_centers", 22, doMsg1);
    ROS_INFO("开始订阅 目标点 /mubiaopoint 话题");
    ros::Subscriber sub2 = nh.subscribe("/mubiaopoint", 22, doMsg2);
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
        ROS_ERROR_STREAM("无法打开串口.");
        return -1;
    }

    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 已打开.");
    }
    else
    {
        return -1;
    }



    
    uint8_t buffer[1024];
    
    const size_t expected_length = 7;
    
    size_t index = 0;
    
    PoseData pose;

    ros::Rate loop_rate(50); 

    while(ros::ok())
    {
        ROS_INFO("进入循环");
        
        size_t n = sp.available();
        if(n != 0)
        {
            ROS_INFO("开始处理串口数据");
            
            n = sp.read(buffer, n);
            for(size_t i = 0; i < n; i++)
            {
                switch(index)
                {
                    
                    case 0:
                        if (buffer[i] == 'z')
                        {
                            ROS_INFO("z");
                            index = 1;
                        }
                        break;
                    
                    case 1:
                        if (buffer[i] >= '1' && buffer[i] <= '6')
                        {
                            pose.task = buffer[i] - '0';
                            ROS_INFO("pose.task=%d",pose.task);
                            index = 2;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    
                    case 2:
                        if (buffer[i] == '0' || buffer[i] == '1')
                        {

                            pose.state = buffer[i] - '0';
                            ROS_INFO("pose.state=%d",pose.state);
                            index = 3;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    
                    case 3:
                        if (buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            pose.key1 = buffer[i] - '0';
                            ROS_INFO("pose.key1=%d",pose.key1);
                            index = 4;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    
                    case 4:
                        if (buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            pose.key2 = buffer[i] - '0';
                            ROS_INFO("pose.key2=%d",pose.key2);
                            index = 5;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    
                    case 5:
                        if (buffer[i] == '0' || buffer[i] == '1')
                        {
                            pose.exec = buffer[i] - '0';
                            ROS_INFO("pose.exec=%d",pose.exec);
                            index = 6;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    
                    case 6:
                        if (buffer[i] == '!')
                        {

                            
                            switch (pose.task)
                            {
                                case 1:
                                    Task1(pose, sp);
                                    break;
                                case 2:
                                    Task2(pose, sp);
                                    break;
                                case 3:
                                    
                                    Task2(pose, sp);
                                    break;
                                case 4:
                                    Task4(pose, sp,pub);
                                    break;
                                case 5:
                                    Task5(pose, sp,pub);
                                    break;
                                case 6:
                                    Task6(pose, sp);
                                    break;
                                default:
                                    ROS_INFO("未知的任务编号: ");
                                    break;
                            }
                            
                            index = 0;
                        }
                        else
                        {
                            
                            index = 0;
                        }
                        break;
                    default:
                        
                        index = 0;
                        break;
                }
            }
        }
        ROS_INFO("回调");
        ros::spinOnce();
        loop_rate.sleep();
    }

    sp.close();
    
    return 0;
}















void Task1(PoseData& pose, serial::Serial& sp)
{
    
    
    
    int x = 11; 
    int y = 22;   

    std::stringstream ss;
    
    ss << "z" << x << "t" << y << "!";
    std::string message = ss.str();
    sp.write(message);
    ROS_INFO_STREAM("放置棋子: " << message);

}

void Task2(PoseData& pose, serial::Serial& sp)
{
    int count=3;
    int x = 1234; 
    int y = 10;   

    switch (pose.judge)
    {
    case 0:
        if(pose.key1!=0)
        {
            
            
        }
        if(pose.key2!=0)
        {
            
            
        }
         if (pose.state == 1) 
        {

            pose.judge = 1;
        }
        break;
    case 1:
        if (pose.state == 1) 
        {
            

        }
        if(pose.state==0)
        {
            pose.key1=0;
            pose.key2=0;
            pose.judge = 2;
        }
        break;
    case 2:
        if(pose.state==0)
        {
            std::stringstream ss;
            ss << "z" << x << "t" << y << "!";
            std::string message = ss.str();
            sp.write(message);
            count--;
            if(count<=0)pose.judge=0;
            ROS_INFO_STREAM("回位: " << message);
        }
        break;
    default:
        break;
    }
}

void Task3(PoseData& pose, serial::Serial& sp)
{
    ROS_INFO_STREAM("正在执行任务 3: state=" << pose.state << ", key1=" << pose.key1 << ", key2=" << pose.key2 << ", exec=" << pose.exec);
}

void Task4(PoseData& pose, serial::Serial& sp,ros::Publisher& pub)
{
    int count=3;
    int x = 1234; 
    int y = 10;   
    switch (pose.judge)
    {
    case 0:
        if(pose.exec == 1 )
        {
            
            

        }

         if (pose.state == 1) 
        {

            pose.judge = 1;
        }
        break;
    case 1:
        if (pose.state == 1) 
        {
           
            std_msgs::Int32 msg;
            msg.data = 1;
            pub.publish(msg);
           
            
            {
                std::unique_lock<std::mutex> lock(mtx);
                 if (!cv.wait_for(lock, std::chrono::seconds(2), [] { return received_data != -1; }))
                {
                    ROS_WARN("超时：未接收到反馈数据。");
                }
            }
            if (received_data != -1)
            {
                ROS_INFO("Received feedback data: %d", received_data);
                

            }
            

        }
        if(pose.state==0)
        {
            pose.key1=0;
            pose.key2=0;
            pose.judge = 2;
        }
        break;
    case 2:
        if(pose.state==0)
        {
            std::stringstream ss;
            ss << "z" << x << "t" << y << "!";
            std::string message = ss.str();
            sp.write(message);
            count--;
            if(count<=0)pose.judge=0;
            ROS_INFO_STREAM("回位: " << message);
        }
        break;
    default:
        break;
    }

}

void Task5(PoseData& pose, serial::Serial& sp,ros::Publisher& pub)
{
    int count=3;
    int x = 1234; 
    int y = 10;   
    switch (pose.judge)
    {
    case 0:
        if(pose.exec == 1 )
        {
            
            

        }

         if (pose.state == 1) 
        {

            pose.judge = 1;
        }
        break;
    case 1:
        if (pose.state == 1) 
        {
           
            std_msgs::Int32 msg;
            msg.data = 1;
            pub.publish(msg);
           
            
            {
                std::unique_lock<std::mutex> lock(mtx);
                 if (!cv.wait_for(lock, std::chrono::seconds(2), [] { return received_data != -1; }))
                {
                    ROS_WARN("超时：未接收到反馈数据。");
                }
            }
            if (received_data != -1)
            {
                ROS_INFO("Received feedback data: %d", received_data);
                

            }
            

        }
        if(pose.state==0)
        {
            pose.key1=0;
            pose.key2=0;
            pose.judge = 2;
        }
        break;
    case 2:
        if(pose.state==0)
        {
            std::stringstream ss;
            ss << "z" << x << "t" << y << "!";
            std::string message = ss.str();
            sp.write(message);
            count--;
            if(count<=0)pose.judge=0;
            ROS_INFO_STREAM("回位: " << message);
        }
        break;
    default:
        break;
    }
}

void Task6(PoseData& pose, serial::Serial& sp)
{
    ROS_INFO_STREAM("正在执行任务 6: state=" << pose.state << ", key1=" << pose.key1 << ", key2=" << pose.key2 << ", exec=" << pose.exec);
}