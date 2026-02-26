#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import ExecuteProcess
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Get installed share directories
    webots_share = get_package_share_directory('webots_env')
    gazebo_share = get_package_share_directory('gazebo_env')

    # Paths to custom resources
    mesh_path = os.path.join(gazebo_share, 'models', 'BioRoboticsLab', 'meshes')
    controllers_path = os.path.join(webots_share, 'controllers')
    world_path = os.path.join(webots_share, 'worlds', 'BioRobotcsLab_2026.wbt')

    # Full path to webots binary (adjust if your installation differs)
    webots_bin = '/usr/local/webots/webots'

    # Get DISPLAY from current environment (or fallback to ':0')
    display = os.environ.get('DISPLAY', ':0')

    # Start with a copy of the current shell environment
    env = os.environ.copy()
    # Add/override our specific variables
    env['WEBOTS_RESOURCES_PATH'] = mesh_path
    env['WEBOTS_CONTROLLER_PATH'] = controllers_path
    env['DISPLAY'] = display
    # Ensure USER is set (optional, avoids warning)
    if 'USER' not in env:
        env['USER'] = 'default'

    webots_cmd = [
        webots_bin,
        '--mode=realtime',
        world_path
    ]

    webots_process = ExecuteProcess(
        cmd=webots_cmd,
        output='screen',
        env=env,
    )

    return LaunchDescription([
        webots_process,
    ])