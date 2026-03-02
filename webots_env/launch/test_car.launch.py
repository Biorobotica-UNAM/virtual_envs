from launch import LaunchDescription
from launch.actions import ExecuteProcess
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('webots_env')

    world_path = os.path.join(pkg_share, 'worlds', 'robot_lab.wbt')
    controllers_path = os.path.join(pkg_share, 'controllers')

    webots_bin = 'webots'  # Assumes Webots is in PATH

    env = os.environ.copy()
    env['WEBOTS_CONTROLLER_PATH'] = controllers_path
    env['DISPLAY'] = os.environ.get('DISPLAY', ':0')

    webots_cmd = [webots_bin, '--mode=realtime', world_path]

    return LaunchDescription([
        ExecuteProcess(
            cmd=webots_cmd,
            output='screen',
            env=env
        )
    ])