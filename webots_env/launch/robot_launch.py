from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable
from ament_index_python.packages import get_package_share_directory
from webots_ros2_driver.webots_launcher import WebotsLauncher
import os

from ament_index_python.packages import get_package_share_directory
import os

webots_share = get_package_share_directory('webots_env')
workspace_install = os.path.dirname(os.path.dirname(webots_share))

def generate_launch_description():
    # Get the installed share directories
    webots_share = get_package_share_directory('webots_env')
    gazebo_share = get_package_share_directory('gazebo_env')

    # Path to the meshes (installed from gazebo_env)
    mesh_path = os.path.join(gazebo_share, 'models', 'BioRoboticsLab', 'meshes')

    print(f"WEBOTS_RESOURCES_PATH set to: {mesh_path}")
    
    # Webots look for assets in this directory
    set_webots_resources = SetEnvironmentVariable(
        name='WEBOTS_RESOURCES_PATH',
        value=mesh_path
    )

    # Launch Webots with the world file
    webots = WebotsLauncher(
        world=os.path.join(webots_share, 'worlds', 'lab_2026.wbt'),
        ros2_supervisor=True   # Enables /clock and supervisor services automatically
    )

    return LaunchDescription([
        set_webots_resources,
        webots,
        # Ros2Supervisor is automatically included with ros2_supervisor=True
    ])