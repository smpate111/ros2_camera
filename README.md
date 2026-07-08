# ros2_camera
Created a ROS2 package that configures a camera into a publisher node.

To run it, enter the command into your terminal:
```
ros2 run ros2_camera camera_pub
```

To view the camera images, run this command in a new terminal (the camera topic will be in the dropdown in the top-left corner after you hit refresh):
```
ros2 run rqt_image_view rqt_image_view
```