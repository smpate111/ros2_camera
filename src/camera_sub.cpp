/*
    This file is a ROS 2 camera subscriber node implemented in C++. It
    subscribes to frames from a camera on a topic.

    The node is defined in the Camera_Subscriber class, which inherits from
    rclcpp::Node. The constructor of this class sets up the subscriber.

    The main function initializes the ROS 2 system, creates an instance of
    the Camera_Subscriber class, and starts spinning to process callbacks
    until the node is shut down.
*/

// Define the node's dependencies.

// The standard C++ headers used in this file.
#include <chrono>
#include <string>

// 3rd party headers used in this file.
#include <opencv2/opencv.hpp>

/*
    The ROS 2 headers used in this file.

    rclcpp/rclcpp.hpp: The main C++ header for ROS 2 client library.
    sensor_msgs/msg/image.hpp: The header for the Image message type.
*/
#include <cv_bridge/cv_bridge.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/header.hpp>

using namespace std::chrono_literals;  // For using time literals like 500ms.

/*
    This class creates a subscriber node that subcribes to frames from a
    topic. It inherits the rclcpp::Node class which is the base class
    used for all ROS 2 nodes in C++.

    The `this` keyword is used to refer to the instance of this class.
*/
class Camera_Subscriber : public rclcpp::Node {
    public:
        /*
            This is the constructor for the Camera_Subscriber class. It initializes
            the node with a name and sets up the subscriber and timer to subscribe
            to frames at defined intervals.

            This constructor is public because we will be calling it from outside
            the class to initialize the node.
        */
        Camera_Subscriber() : Node("Camera_Subscriber") {
            // This callback function is called whenever a new frame is received on the subscribed topic.
            auto topic_callback =
                [this](sensor_msgs::msg::Image::UniquePtr msg) -> void {
                    RCLCPP_INFO_ONCE(
                        this->get_logger(),
                        "Received frame. Opening window."
                    );

                    // Convert the ROS frame message to an OpenCV frame.
                    cv::Mat cv_frame = cv_bridge::toCvCopy(*msg, "bgr8")->image;

                    // Display the frame in a window.
                    cv::imshow("Camera Window", cv_frame);
                    cv::waitKey(1);
                };

            // Create a subscription that listens for frames on the subscribed topic.
            subscriber_ =
                this->create_subscription<sensor_msgs::msg::Image>(
                    "Topic_Camera_Frame",
                    20,
                    topic_callback
                );
        }
    
    private:
        // Declare the subscriber variable as a private member because
        // it is only used inside this class.
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscriber_;
};

int main (int argc, char * argv[]) {
    /*
        The main function initializes the ROS 2 system, creates an instance of the
        Camera_Subscriber class, and starts spinning to process callbacks until
        the node is shut down.
    */
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Camera_Subscriber>());
    rclcpp::shutdown();

    return 0;
}