^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package tmc_utils
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.1.0 (2025-12-04)
-------------------
* Use an another node to avoid exceptions when calling get_parameter.
* Added function to allow setting multiple JointTrajectoryController instances as successors.
* Adapt DynamicParameter to LifecycleNode.
* Add utils for chainable controller
* Add DynamicParameter to tmc_utils_py
* Add ResolveRobotDescription to tmc_utils
* Add get_param_path in tmc_launch_ros_utils.
* Add COLCON_IGNORE to exclude ROS1 packages from colcon build
* Make DynamicParameter thread-safe
* Make CachingSubscriber thread-safe
* Removed elements that can be obtained from the subscriber object
* Deleted wait_for_connections and correction of typos
* Converted caching_subscriber.py to ROS2
* Fixed setup.py
* Converting tmc_utils_py to ros2
* Deleted package.xml depend
* Fixed lint tmc_mqtt_bridge
* Fixed CMakeLists.txt and package.xml
* Convert tmc_mqtt_bridge to ros2
* Modify to accept uppercase STL file extensions
* apply humble to tmc_eigen_bridge
* Contributors: Hiroaki Yaguchi, Hiroki Nakano, Keisuke Takeshita, Yuki Hidaka, ryu nishimori

2.0.0 (2024-10-03)
-------------------
* Initial release
* Contributors: Keisuke Takeshita

