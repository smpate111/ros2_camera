cmake_minimum_required(VERSION 3.8)
project(ros2_camera)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# create libraries
add_library(camera_pub_library src/camera_pub.cpp)
add_library(camera_sub_library src/camera_sub.cpp)

target_include_directories(
  camera_pub_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(
  camera_sub_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(camera_pub_library cv_bridge image_transport OpenCV rclcpp sensor_msgs std_msgs)
ament_target_dependencies(camera_sub_library cv_bridge image_transport OpenCV rclcpp sensor_msgs std_msgs)

# create executables
add_executable(camera_pub src/camera_pub_node.cpp)
add_executable(camera_sub src/camera_sub_node.cpp)

ament_target_dependencies(camera_pub cv_bridge image_transport OpenCV rclcpp sensor_msgs std_msgs)
ament_target_dependencies(camera_sub cv_bridge image_transport OpenCV rclcpp sensor_msgs std_msgs)

# link executables to libraries
target_link_libraries(camera_pub camera_pub_library)
target_link_libraries(camera_sub camera_sub_library)

install(
  TARGETS camera_pub camera_sub
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()