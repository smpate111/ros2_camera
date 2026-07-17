/*
    This file defines a ROS 2 publisher node in C++. This node
    publishes frames to a camera topic.
*/

#ifndef ROS2_CAMERA__CAMERA_PUB_HPP_
#define ROS2_CAMERA__CAMERA_PUB_HPP_

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
    This class creates a publisher node that publishes frames to a camera topic. 
    It inherits the rclcpp::Node class which is the base class used for all ROS
    2 nodes created in C++.
*/
class Camera_Publisher : public rclcpp::Node {
    /*
        The following function is declared as a public member of this class
        because we want to make sure that we can initialize this node from
        outside this class.
    */
    public:
        Camera_Publisher();

    /*
        The following variables and functions are declared as protected members
        of this class because we want to make sure they are only accessible
        inside the parent class and child class(es). There is no reason for
        us to call them outside those classes because we don't need to access
        the logic from there anyways.
    */
    protected:
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
        size_t count_;
        cv::VideoCapture cap_;
        cv::Mat camera_frame;
        int camera_index = 0;   // Change this value to whatever index your OS assigns your camera.

        bool open_camera();     // Checks if the camera is open to determine if the publisher should run.
        void timer_callback();  // Executes the following 2 functions at a defined interval.
        bool capture_frame();   // Captures camera frames from web camera.
        void publish_frame();   // Publishes camera frames to a camera topic.
};

#endif