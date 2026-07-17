/*
    This file initializes the ROS 2 system, creates an instance of the
    Camera_Subscriber node, and starts spinning to process callbacks
    until the node is shut down.
*/

// Include the header file.
#include <ros2_camera/camera_sub.hpp>

int main (int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Camera_Subscriber>());
    rclcpp::shutdown();

    return 0;
}