from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, SetEnvironmentVariable
from launch.substitutions import Command, PathJoinSubstitution, LaunchConfiguration
from launch_ros.actions import Node
#from launch_ros.substitutions import FindPackageShare
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():

# ================== ENVIRONMENT SETUP =================== #


    #CHANGE THESE TO BE RELEVANT TO THE SPECIFIC PACKAGE
    robot_description_path = get_package_share_directory('justina_description')  # -----> Change me!
    gazebo_env_pkg = get_package_share_directory('gazebo_env')
    custom_world_file_name = 'lab_2026.sdf'

    parent_of_share_path = os.path.dirname(robot_description_path)


    webots_env_share = get_package_share_directory('webots_env')
    models_path = os.path.join(webots_env_share, 'models', 'BioRoboticsLab')

    set_gz_sim_resource_path = SetEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=[
            os.environ.get('GZ_SIM_RESOURCE_PATH', ''),
            os.path.pathsep,
            models_path
        ]
    )


    # --- Use sim time setup ---
    use_sim_time_declare = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation (Gazebo) clock if true'
    )
    use_sim_time = LaunchConfiguration('use_sim_time')




# ============== GAZEBO - SETUP AND LAUNCH ================ #

    
    # Include the Gazebo Sim launch file (using gz_sim.launch.py)
    gz_sim_launch_file = os.path.join(
    get_package_share_directory('ros_gz_sim'),
    'launch',
    'gz_sim.launch.py'
    )

    # Define the path to your custom world file
    custom_world_path = os.path.join(
        gazebo_env_pkg,
        'worlds',
        custom_world_file_name
    )


    # Construct the gz_args to the custom world as a single string
    gz_args_value = f'-r {custom_world_path}'

    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(gz_sim_launch_file),
        launch_arguments={'gz_args': gz_args_value, 
                          'use_sim_time': use_sim_time,
                          'on_exit_shutdown': 'True' 
                          }.items()
    )


    return LaunchDescription([
        set_gz_sim_resource_path,
        use_sim_time_declare,
        gazebo_launch,
    ])
