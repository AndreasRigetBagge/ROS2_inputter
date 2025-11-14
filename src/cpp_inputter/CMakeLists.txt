cmake_minimum_required(VERSION 3.8)
project(cpp_inputter)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

### find dependencies
#further ones can be added here depending on need
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED) 

###adding the executable and its dependencies for 'ros2 run' command (?)
add_executable(input_publisher src/inputPublisher.cpp)
ament_target_dependencies(input_publisher rclcpp std_msgs geometry_msgs turtlesim)

###make sure 'ros2 run' command can find executable:
install(TARGETS
  input_publisher
  DESTINATION lib/${PROJECT_NAME})

ament_package()
