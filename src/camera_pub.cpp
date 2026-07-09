/*
    This file is a ROS 2 camera publisher node implemented in C++. It
    publishes frames from a camera to a topic at a regular interval
    (this is adjustable to your needs).

    The node is defined in the Camera_Publisher class, which inherits from
    rclcpp::Node. The constructor of this class sets up the publisher and
    a timer that triggers the publishing of these frames.

    The main function initializes the ROS 2 system, creates an instance of
    the Camera_Publisher class, and starts spinning to process callbacks
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
    This class creates a publisher node that publishes frames to a
    topic. It inherits the rclcpp::Node class which is the base class
    used for all ROS 2 nodes in C++.

    The `this` keyword is used to refer to the instance of this class.
*/
class Camera_Publisher : public rclcpp::Node {
    public:
        /*
            This is the constructor for the Camera_Publisher class. It initializes
            the node with a name and sets up the publisher and timer to publish
            frames at defined intervals.

            This constructor is public because we will be calling it from outside
            the class to initialize the node.
        */
        Camera_Publisher() : Node("Camera_Publisher"), count_(0) {
            // Creating a publisher that publishes frames to the topic with a queue size of 20.
            publisher_ = this->create_publisher<sensor_msgs::msg::Image>("Topic_Camera_Frame", 20);

            // Open camera before doing anything else
            bool is_camera_open = open_camera();
            if (is_camera_open == false) {
                return;
            }

            // This timer calls the timer_callback function at a defined interval.
            timer_ =
                this->create_wall_timer(
                    25ms,
                    std::bind(
                        &Camera_Publisher::timer_callback,
                        this
                    )
                );
        }
    
    private:
        // Declare these variables as private members
        // because they are only used inside this class.
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
        size_t count_;
        cv::VideoCapture cap_;
        cv::Mat camera_frame;
        int camera_index = 0;   // Change this value to whatever index your OS assigns your camera.

        // Open the camera and find out if it can open before sending frames.
        bool open_camera() {
            cap_.open(camera_index);

            // Don't execute camera capture if camera is not open.
            if (!cap_.isOpened()) {
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Error: Could not open camera with index %d.",
                    camera_index
                );

                return false;
            }

            RCLCPP_INFO(
                this->get_logger(),
                "Success: Camera with index %d opened.",
                camera_index
            );

            return true;
        }

        /*
            This function captures and publishes frames to a topic
            at a defined interval.
        */
        void timer_callback() {
            bool is_frame_captured = capture_frame();
            if (is_frame_captured == false) {
                return;
            }

            publish_frame();

            return;
        }

        /*
            This function captures a frame from the camera. It checks
            if the camera is open and the frame is not empty before
            it gets published to the topic.
        */
        bool capture_frame() {
            // Skip capturing frame if camera is closed.
            if (!cap_.isOpened()) {
                RCLCPP_WARN_THROTTLE(
                    this->get_logger(),
                    *this->get_clock(),
                    2000,
                    "Camera is not opened. Skipping current frame capture."
                );

                return false;
            }

            // Capture camera frame.
            cap_ >> camera_frame;

            // Skip current frame if it is empty.
            if (camera_frame.empty()) {
                RCLCPP_WARN(
                    this->get_logger(),
                    "Warning: Captured empty frame from the camera. Skipping current frame."
                );

                return false;
            }

            return true;
        }

        /*
            This function publishes the captured frame to a topic.
        */
        void publish_frame() {
            std_msgs::msg::Header header;
            header.stamp = this->now();
            header.frame_id = "Camera_Frame";

            auto frame_message = cv_bridge::CvImage(
                header,
                "bgr8",
                camera_frame
            ).toImageMsg();

            RCLCPP_INFO(
                this->get_logger(),
                "Publishing Camera Frame %zu.",
                this->count_++
            );
            
            publisher_->publish(*frame_message);
            return;
        }
};

int main (int argc, char * argv[]) {
    /*
        The main function initializes the ROS 2 system, creates an instance of the
        Camera_Publisher class, and starts spinning to process callbacks until
        the node is shut down.
    */
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Camera_Publisher>());
    rclcpp::shutdown();

    return 0;
}