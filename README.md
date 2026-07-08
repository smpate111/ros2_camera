# ros2_camera
Created a ROS 2 package that configures a camera into a publisher node. If you plan on using this package, then make sure you are using the ROS 2 Jazzy distribution (this package has not been tested in the other distributions). Additionally, assign the value of the `int camera_index` object to the index your OS assigns to your camera or the program will not run.

The packages you need to install in order to run this:
```
snapd
kamoso
OpenCV
```

The ROS 2 packages you need to install in order to run this:
```
rclcpp
image_transport
cv_bridge
sensor_msgs
std_msgs
```

To run it, enter the command into your terminal:
```
ros2 run ros2_camera camera_pub
```

In a new terminal, enter this command:
```
ros2 run ros2_camera camera_sub
```