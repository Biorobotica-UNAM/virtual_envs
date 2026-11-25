# An array of poses that represents a path for robot.

# Header
std_msgs/Header header

# Pose sequence
geometry_msgs/PoseStamped[] poses

# Goal pose
geometry_msgs/Pose goal

# Index of robot current position for HMI
uint32 current_position

