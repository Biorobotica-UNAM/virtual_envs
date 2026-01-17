cmake_minimum_required(VERSION 3.8)
project(gazebo_env)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(ros_gz_sim REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)

# Linting (optional)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Install directories
# Launch files
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/launch
)

# Worlds
install(DIRECTORY worlds
  DESTINATION share/${PROJECT_NAME}/worlds
)

# Models
install(DIRECTORY models
  DESTINATION share/${PROJECT_NAME}/models
)

ament_package()
