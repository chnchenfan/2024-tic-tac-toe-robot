cmake_minimum_required(VERSION 2.8.3)
project(topic_example)







find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  std_msgs
)


catkin_package(
  INCLUDE_DIRS include



)







include_directories(

  ${catkin_INCLUDE_DIRS}
)



add_executable(publish_node src/publish_node.cpp
			    src/mbot_linux_serial.cpp)
target_link_libraries(publish_node ${catkin_LIBRARIES})

