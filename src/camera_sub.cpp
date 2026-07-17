/*
    This file fleshes out the subscriber node logic from
    camera_sub.hpp.
*/

// Include the header file.
#include <ros2_camera/camera_sub.hpp>

/*
    This is the constructor for the Camera_Subscriber class. It initializes
    the node with a name and sets up the subscriber to get the frames from
    the subscribed topic.
*/
Camera_Subscriber::Camera_Subscriber() : Node("Camera_Subscriber") {
    subscriber_ =
        this->create_subscription<sensor_msgs::msg::Image>(
            "Topic_Camera_Frame",
            20,
            std::bind(
                &Camera_Subscriber::frame_callback,
                this,
                std::placeholders::_1
            )
        );
};

/*
    This is the callback function that displays frame from a camera topic in
    an OpenCV window. If the current frame has issues, then it gets skipped.
*/
void Camera_Subscriber::frame_callback(const sensor_msgs::msg::Image::SharedPtr msg) {
    try {
        RCLCPP_INFO_ONCE(
                this->get_logger(),
                "Received frame. Opening window."
            );
        
        // Display the frame in a window.
        cv::Mat cv_frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
        cv::imshow("Camera Window", cv_frame);
        cv::waitKey(1);
    } catch (cv_bridge::Exception &e) {
        RCLCPP_ERROR(
            this->get_logger(),
            "cv_bridge Exception: %s",
            e.what()
        );
    }

    return;
}