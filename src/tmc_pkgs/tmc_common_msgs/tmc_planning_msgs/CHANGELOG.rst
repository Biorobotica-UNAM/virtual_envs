^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package tmc_planning_msgs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.2.0 (2025-12-04)
-------------------
* Adjust interfaces to remove robot-specific dependencies.
* Remove enable_arm/head/gripper from RobotLocalGoal.msg
* Add constant INVALID_INPUT_ROBOT_STATE to RobotLocalPlannerStatus.msg.
* Add recognized msg to tmc_vision_msgs for humble.
* Contributors: Keisuke Takeshita, 柴宮 和希

2.1.0 (2025-04-22)
-------------------
* comment out unused msg/srv
* tmc_vision_msgs ROS 2 humble version
* Move srvs from tmc_base_path_follower.
* Add tmc_planning_msgs/msg/LinearConstraintWithPose.msg and update tmc_planning_msgs/msg/Constraints.msg
* remove unused msgs from build target, revert unused msgs
* enamble messages for safe_pose_changer
* Contributors: Hiroaki Yaguchi, Keisuke Takeshita, sawada

2.0.0 (2024-10-11)
-------------------
* Initial release
* Contributors: Keisuke Takeshita, Koji Terada, Yuta Watanabe

