cmake_minimum_required(VERSION 3.8)
project(gazebo_env)

# Default to C++17 for ROS 2 Jazzy
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)

# Find packages (these are mostly for build dependencies)
# Remove launch and launch_ros from find_package - they're Python packages

# Linting (optional)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Install world file
install(FILES worlds/lab_2026.sdf
  DESTINATION share/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/
)

# Install model files
install(DIRECTORY models/
  DESTINATION share/${PROJECT_NAME}/
)

# If you have config files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config")
  install(DIRECTORY config/
    DESTINATION share/${PROJECT_NAME}/
  )
endif()

# Install Python scripts
install(PROGRAMS 
  scripts/generate_urdf.py
  scripts/publish_joint_states.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()