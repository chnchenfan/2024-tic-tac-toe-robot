cmake_minimum_required(VERSION 3.0.2)
project(color_dected)







find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  geometry_msgs
  image_transport
  moveit_core
  moveit_ros_planning
  moveit_ros_planning_interface
  moveit_visual_tools
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf2
  tf2_geometry_msgs
  tf2_ros
)


























































































catkin_package(




)







include_directories(

  ${catkin_INCLUDE_DIRS}
)








































catkin_install_python(PROGRAMS
  scripts/color_dected.py
  scripts/color_dectednew.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)









































