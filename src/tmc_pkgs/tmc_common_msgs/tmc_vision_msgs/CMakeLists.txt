cmake_minimum_required(VERSION 3.8)
project(tmc_vision_msgs)

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
# find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tmc_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

set(msg_files
  # "msg/ContourSubPixel.msg"
  # "msg/ContourSubPixelArray.msg"
  # "msg/Detection.msg"
  # "msg/DetectionArray.msg"
  # "msg/DetectionWithMask.msg"
  # "msg/DetectionWithMaskArray.msg"
  # "msg/HumanPose.msg"
  # "msg/HumanPoseArray.msg"
  # "msg/Keypoint.msg"
  # "msg/Label.msg"
  # "msg/LabelArray.msg"
  # "msg/MarkerArray.msg"
  "msg/RecognitionMethod.msg"
  "msg/RecognizedObject.msg"
  # "msg/Segmentation.msg"
  # "msg/SegmentationArray.msg"
  # "msg/SquareMarkerCorners.msg"
  # "msg/SquareMarkerCornersArray.msg"
  # "msg/StereoMarkerVertices.msg"
  # "msg/StereoMarkerVerticesNumbered.msg"
  # "msg/StereoMarkerVerticesNumberedArray.msg"
  # "msg/SubPixel.msg"
)

set(srv_files
  "srv/ControlCamera.srv"
  # "srv/DepthSegmentation.srv"
  # "srv/Flip.srv"
  # "srv/SetTargets.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES builtin_interfaces
  geometry_msgs
  # sensor_msgs
  std_msgs
  tmc_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
