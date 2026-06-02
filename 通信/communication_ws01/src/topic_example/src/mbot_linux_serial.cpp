#include "mbot_linux_serial.h"
#include <boost/asio.hpp> 

using namespace std;
using namespace boost::asio;


unsigned char servo_sdata[20] = { 0x7b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7d };  

void serialInit(boost::asio::serial_port& sp)
{
    sp.set_option(serial_port::baud_rate(115200));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));
}





void writeSpeed2(boost::asio::serial_port& sp, const std::vector<int>& data)
{
    if (data.size() != 18) {
        ROS_ERROR("Data size must be 18");
        return;
    }
    for(int i = 0; i < 18 ; i++)
    {
        ROS_INFO("data: ");
        ROS_INFO("%d",data[i]);
    }

    unsigned char extended_servo_sdata[38]; 
    extended_servo_sdata[0] = 123;  

    for (size_t i = 0; i < 18; ++i) {
        extended_servo_sdata[2 * i + 1] = static_cast<unsigned char>(data[i] & 0xFF); 
        extended_servo_sdata[2 * i + 2] = static_cast<unsigned char>((data[i] >> 8) & 0xFF); 
    }

    extended_servo_sdata[37] = 125; 

    ROS_INFO("Sending data: ");
    for (size_t i = 0; i < 38; ++i) {
        ROS_INFO("%d ", extended_servo_sdata[i]);
    }

    
    boost::asio::write(sp, boost::asio::buffer(extended_servo_sdata, 38));
}





void writeSpeed3(boost::asio::serial_port& sp, const std::vector<int>& data)
{
    if (data.size() != 2) {
        ROS_ERROR("Data size must be 2");
        return;
    }

    unsigned char extended_servo_sdata[6]; 
    extended_servo_sdata[0] = 123;  

    for (size_t i = 0; i < 2; ++i) {
        extended_servo_sdata[2 * i + 1] = static_cast<unsigned char>(data[i] & 0xFF); 
        extended_servo_sdata[2 * i + 2] = static_cast<unsigned char>((data[i] >> 8) & 0xFF); 
    }

    extended_servo_sdata[5] = 125; 

    ROS_INFO("Sending data: ");
    for (size_t i = 0; i < 6; ++i) {
        ROS_INFO("%d ", extended_servo_sdata[i]);
    }

    
    boost::asio::write(sp, boost::asio::buffer(extended_servo_sdata, 6));
}






void writeSpeed4(boost::asio::serial_port& sp, const std::vector<int>& data)
{
    if (data.size() != 18) {
        ROS_ERROR("Data size must be 18");
        return;
    }
    for(int i = 0; i < 18 ; i++)
    {
        ROS_INFO("data: ");
        ROS_INFO("%d",data[i]);
    }
    
    unsigned char extended_servo_sdata[38]; 
    extended_servo_sdata[0] = 255;  

    for (size_t i = 0; i < 18; ++i) {
        extended_servo_sdata[2 * i + 1] = static_cast<unsigned char>(data[i] & 0xFF); 
        extended_servo_sdata[2 * i + 2] = static_cast<unsigned char>((data[i] >> 8) & 0xFF); 
    }

    extended_servo_sdata[37] = 254; 

    ROS_INFO("Sending data: ");
    for (size_t i = 0; i < 38; ++i) {
        ROS_INFO("%d ", extended_servo_sdata[i]);
    }

    
    boost::asio::write(sp, boost::asio::buffer(extended_servo_sdata, 38));
}