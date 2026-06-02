cmake_minimum_required(VERSION 3.0.2)
project(sub_pub)







find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  serial
  message_generation
)



































add_message_files(
  FILES
  pose.msg
)
















generate_messages(
  DEPENDENCIES
  std_msgs
)






























catkin_package(


 CATKIN_DEPENDS roscpp rospy std_msgs message_runtime

)







include_directories(

  ${catkin_INCLUDE_DIRS}
)








































catkin_install_python(PROGRAMS
  scripts/control_pub.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)










































add_executable(serial_stm32 src/serial_stm32.cpp)

add_dependencies(serial_stm32 ${PROJECT_NAME}_generate_messages_cpp)

target_link_libraries(serial_stm32
  ${catkin_LIBRARIES}
)