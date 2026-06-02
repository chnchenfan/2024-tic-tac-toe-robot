#include "mbot_linux_serial.h"
#include <boost/asio.hpp> 

using namespace std;
using namespace boost::asio;




boost::system::error_code err;





unsigned char servo_sdata[6] = { 0x7b,0,0,0,0,0x7d };
unsigned char blue_sdata[3] = { 0xFF, 0, 0xFE };



















void serialInit(boost::asio::serial_port& sp)
{
    sp.set_option(serial_port::baud_rate(115200));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));    
}






void writeSpeed1(boost::asio::serial_port& sp, int data1,int data2,int data3)
{
	servo_sdata[0] = 123;  
	servo_sdata[1] = (int)data1;
	servo_sdata[2] = (int)data2;   
	servo_sdata[3] = (int)data3;
	



	servo_sdata[5] = 125; 


	ROS_INFO("%d %d %d %d ",servo_sdata[1],servo_sdata[2],servo_sdata[3],servo_sdata[4]);
    
    boost::asio::write(sp, boost::asio::buffer(servo_sdata));
}
void writeSpeed2(boost::asio::serial_port& sp)
{
	blue_sdata[0] = 0xFF;  
	blue_sdata[1] = 0x08;
	blue_sdata[2] = 0xFE;   

	ROS_INFO("%d %d %d",blue_sdata[0],blue_sdata[1],blue_sdata[2]);
    
    boost::asio::write(sp, boost::asio::buffer(blue_sdata));
}











    









































































    





























