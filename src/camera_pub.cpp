/*
    This file fleshes out the publisher node logic
    from camera_pub.hpp.
*/

// Include the header file.
#include <ros2_camera/camera_pub.hpp>

/*
    This is the constructor for the Camera_Publisher class. It initializes
    the node with a name and sets up the publisher and timer to publish
    frames at defined intervals.
*/
Camera_Publisher::Camera_Publisher() : Node("Camera_Publisher"), count_(0) {
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

// Open the camera and find out if it can open before sending frames.
bool Camera_Publisher::open_camera() {
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
void Camera_Publisher::timer_callback() {
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
bool Camera_Publisher::capture_frame() {
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
void Camera_Publisher::publish_frame() {
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