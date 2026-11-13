# Error codes related to manipulation

# Type of error
int32 val


# Overall behavior

# The planning is failed
int32 PLANNING_FAILED=-1
# The action is success
int32 SUCCESS=1
# The action is failed by timeout
int32 TIMED_OUT=-2


# Start state errors

# The initial robot state is in collision
int32 START_STATE_IN_COLLISION=-3
# The initial robot state is out of constraints
int32 START_STATE_VIOLATES_PATH_CONSTRAINTS=-4


# Goal errors

# The final robot state is in collision
int32 GOAL_IN_COLLISION=-5
# The final robot state is out of constraints
int32 GOAL_VIOLATES_PATH_CONSTRAINTS=-6


# Robot state

# The robot state is invalid
int32 INVALID_ROBOT_STATE=-7
# The robot state is not complete
int32 INCOMPLETE_ROBOT_STATE=-8


# Planning request errors

# The planner id is invalid
int32 INVALID_PLANNER_ID=-9
# The number of planning attempts is invalid
int32 INVALID_NUM_PLANNING_ATTEMPTS=-10
# The planning time is invalid
int32 INVALID_ALLOWED_PLANNING_TIME=-11
# The group name does not exist
int32 INVALID_GROUP_NAME=-12
# The constraints of final joint state is invalid
int32 INVALID_GOAL_JOINT_CONSTRAINTS=-13
# The constraints of final frame's position is invalid
int32 INVALID_GOAL_POSITION_CONSTRAINTS=-14
# The constraints of final frame's orientation is invalid
int32 INVALID_GOAL_ORIENTATION_CONSTRAINTS=-15
# The path constraints of joint state is invalid
int32 INVALID_PATH_JOINT_CONSTRAINTS=-16
# The path constraints of frame's position is invalid
int32 INVALID_PATH_POSITION_CONSTRAINTS=-17
# The path constraints of frame's orientation is invalid
int32 INVALID_PATH_ORIENTATION_CONSTRAINTS=-18


# State/trajectory monitor errors

# The trajectory is invalid
int32 INVALID_TRAJECTORY=-19
# Size mismatch is detected
int32 INVALID_INDEX=-20
# The joint limits are violated
int32 JOINT_LIMITS_VIOLATED=-21
# The path constraints are violated
int32 PATH_CONSTRAINTS_VIOLATED=-22
# The collision constraints are violated
int32 COLLISION_CONSTRAINTS_VIOLATED=-23
# The goal constraints are violated
int32 GOAL_CONSTRAINTS_VIOLATED=-24
# A joint is requested to move, but the joint does not move
int32 JOINTS_NOT_MOVING=-25
# The trajectory controller is failed
int32 TRAJECTORY_CONTROLLER_FAILED=-26


# System errors

# Computation of trasform is failed
int32 FRAME_TRANSFORM_FAILURE=-27
# Collision detection does not work
int32 COLLISION_CHECKING_UNAVAILABLE=-28
# The system cannot receive the robot state
int32 ROBOT_STATE_STALE=-29
# The system cannot receive any sensor's output
int32 SENSOR_INFO_STALE=-30


# Kinematics errors

# The requests contain some robot states which cannot be solved IK
int32 NO_IK_SOLUTION=-31
# The requests contain invalid link
int32 INVALID_LINK_NAME=-32
# IK is solved, but it is in collision
int32 IK_LINK_IN_COLLISION=-33
# FK is not solved
int32 NO_FK_SOLUTION=-34
# The kinematic robot state is in collision
int32 KINEMATICS_STATE_IN_COLLISION=-35

# The shortcutting process is failed by timeout
int32 SHORTCUT_TIMEDOUT=-36
# The timeout is invalid
int32 INVALID_TIMEOUT=-37
