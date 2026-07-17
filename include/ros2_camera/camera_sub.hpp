/*
    This file defines a ROS 2 subscriber node in C++. This node
    subscribes to a camera topic and displays the frames in a
    window.
*/

#ifndef ROS2_CAMERA__CAMERA_SUB_HPP_
#define ROS2_CAMERA__CAMERA_SUB_HPP_

// Define the node's dependencies.

// The standard C++ headers used in this file.
#include <chrono>
#include <string>

// 3rd party headers used in this file.
#include <opencv2/opencv.hpp>

// The ROS 2 headers used in this file.
#include <cv_bridge/cv_bridge.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/header.hpp>

using namespace std::chrono_literals;  // For using time literals like 500ms.

/*
    This class creates a subscriber node that subscribes to a camera topic and
    displays them in a window. It inherits the rclcpp::Node class which is the
    base class used for all ROS 2 nodes created in C++.
*/
class Camera_Subscriber : public rclcpp::Node {
    /*
        The following function is declared as a public member of this class
        because we want to make sure that we can initialize this node from
        outside this class.
    */
    public:
        Camera_Subscriber();

    /*
        The following variable and function are declared as protected members
        of this class because we want to make sure they are only accessible
        inside the parent class and child class(es). There is no reason for
        us to call them outside those classes because we don't need to access
        the logic from there anyways.
    */
    protected:
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscriber_;
        void frame_callback(const sensor_msgs::msg::Image::SharedPtr msg);
};

#endif