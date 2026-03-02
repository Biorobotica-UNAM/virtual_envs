#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import ExecuteProcess
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    webots_share = get_package_share_directory('webots_env')
    justina_share = get_package_share_directory('justina_description')
    webots_ros2_share = get_package_share_directory('webots_ros2_driver')

    # Parent directories (contain the package folders)
    justina_base = os.path.dirname(justina_share)
    webots_ros2_base = os.path.dirname(webots_ros2_share)

    extra_projects = f"{justina_base}:{webots_ros2_base}"
    print(f"WEBOTS_EXTRA_PROJECT_PATH = {extra_projects}")  # debug

    controllers_path = os.path.join(webots_share, 'controllers')
    world_path = os.path.join(webots_share, 'worlds', 'configuring_robot.wbt')

    webots_bin = 'webots'   # Assumes webots is in PATH

    env = os.environ.copy()
    env['WEBOTS_EXTRA_PROJECT_PATH'] = extra_projects
    env['WEBOTS_CONTROLLER_PATH'] = controllers_path
    env['DISPLAY'] = os.environ.get('DISPLAY', ':0')
    if 'USER' not in env:
        env['USER'] = 'default'

    webots_cmd = [webots_bin, '--mode=realtime', world_path]

    webots_process = ExecuteProcess(
        cmd=webots_cmd,
        output='screen',
        env=env,
    )

    return LaunchDescription([webots_process])