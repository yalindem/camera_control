cmake_minimum_required(VERSION 3.8)
project(gstreamer_camera)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_COLOR_MAKEFILE ON)

find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(image_transport REQUIRED)

add_executable(camera_node src/gstreamer_camera_node.cpp)
ament_target_dependencies(camera_node rclcpp sensor_msgs cv_bridge image_transport OpenCV)
install(TARGETS
  camera_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
