cmake_minimum_required(VERSION 3.8)
project(tmc_navigation_msgs)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

set(msg_files
  "msg/BaseBehaviorState.msg"
  "msg/BaseBehaviorStatus.msg"
  "msg/BaseFinish.msg"
  "msg/BaseLocalPlannerStatus.msg"
  "msg/BasePathFollowerStatus.msg"
  "msg/Goal.msg"
  "msg/LocalizationStatus.msg"
  "msg/OccupancyGridUint.msg"
  "msg/PathWithGoal.msg"
  "msg/SurroundingObjectInfo.msg"
  "msg/SurroundingObjectInfoArray.msg"
  "msg/TfNameIdentifier.msg"
  "msg/Waypoint.msg"
)

set(srv_files
  "srv/BoolResponse.srv"
  "srv/ChangeParameter.srv"
  "srv/FunctionSwitch.srv"
  "srv/GetCurrentSetting.srv"
  "srv/GetParameter.srv"
  "srv/OdometrySwitch.srv"
  "srv/Pause.srv"
  "srv/PausePathFollow.srv"
  "srv/PauseSwingLrf.srv"
  "srv/PlayPause.srv"
  "srv/PlayReach.srv"
  "srv/PlayStop.srv"
  "srv/PointCloudLevelOffset.srv"
  "srv/ReloadMap.srv"
  "srv/ReloadParam.srv"
  "srv/ResumeSwingLrf.srv"
  "srv/RoadRecognizerEnableSwitch.srv"
  "srv/Run.srv"
  "srv/RunWithBaseGlobalPlan.srv"
  "srv/SetBaseLocalGoal.srv"
  "srv/SetLocalizationScoreLimit.srv"
  "srv/SetMovingDirectionMode.srv"
  "srv/SetNoiseFilterParam.srv"
  "srv/SetPositionAdjustmentMethod.srv"
  "srv/SetTrackTarget.srv"
  "srv/Stop.srv"
  "srv/SwitchBumperSet.srv"
  "srv/Twist.srv"
)

set(action_files
  "action/BasePathPlan.action"
  "action/Empty.action"
  "action/PathFollower.action"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  ${action_files}
  DEPENDENCIES builtin_interfaces geometry_msgs nav_msgs std_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
