#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, SetEnvironmentVariable, TimerAction
from launch.substitutions import Command, PathJoinSubstitution, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    
    # ================== CRITICAL: GAZEBO ENVIRONMENT SETUP =================== #
    
    # Get package paths
    justina_desc_path = get_package_share_directory('justina_description')
    gazebo_env_pkg = get_package_share_directory('gazebo_env')
    webots_env_share = get_package_share_directory('webots_env')
    models_path = os.path.join(webots_env_share, 'models', 'BioRoboticsLab')

    # Build the complete resource path for Gazebo
    # This is what makes textures load correctly
    resource_paths = [
        '/opt/ros/jazzy/share',                     # ROS 2 base directory
        justina_desc_path,                          # Our package directory
        os.path.join(justina_desc_path, 'meshes', 'head'),  # Texture directory
        os.path.dirname(justina_desc_path),         # Parent directory
        models_path,                                # Webots models directory
    ]
    
    # Remove any empty paths and join
    resource_paths = [p for p in resource_paths if p]
    gz_resource_path = os.path.pathsep.join(resource_paths)
    
    # Create environment setup
    env_setup = [
        # Main resource path for Gazebo Sim
        SetEnvironmentVariable(
            name='GZ_SIM_RESOURCE_PATH',
            value=gz_resource_path
        ),
        # Legacy Gazebo paths for compatibility
        SetEnvironmentVariable(
            name='GAZEBO_RESOURCE_PATH',
            value=os.path.join(justina_desc_path, 'meshes', 'head')
        ),
        SetEnvironmentVariable(
            name='GAZEBO_MODEL_PATH',
            value=os.path.join(justina_desc_path, 'meshes', 'head')
        ),
    ]
    
    # ================== LAUNCH ARGUMENTS =================== #
    
    use_sim_time_declare = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation (Gazebo) clock if true'
    )
    use_sim_time = LaunchConfiguration('use_sim_time')
    
    # Robot spawn position at (0, 1, 0.1)
    x_pos = DeclareLaunchArgument('x', default_value='0.0')
    y_pos = DeclareLaunchArgument('y', default_value='1.0')
    z_pos = DeclareLaunchArgument('z', default_value='0.1')
    yaw = DeclareLaunchArgument('yaw', default_value='0.0')

    # ============== ROBOT DESCRIPTION ================ #
    
    # Generate robot description from xacro
    # Uses package:// URI which is portable
    robot_description_content = Command([
        'xacro ',
        PathJoinSubstitution([
            FindPackageShare('justina_description'),
            'urdf','justina_gazebo',
            'justina.xacro'
        ])
    ])
    
    # Robot State Publisher
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[{
            'use_sim_time': use_sim_time,
            'robot_description': robot_description_content
        }]
    )
    
    # Joint State Publisher
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'source_list': ['joint_states']
        }]
    )

    # ============== GAZEBO SIMULATION ================ #
    
    # Path to Gazebo Sim launch file
    gz_sim_launch_file = os.path.join(
        get_package_share_directory('ros_gz_sim'),
        'launch',
        'gz_sim.launch.py'
    )
    
    # Path to custom world
    custom_world_path = os.path.join(
        gazebo_env_pkg,
        'worlds',
        'lab_2026.sdf'
    )
    
    # Arguments for Gazebo
    gz_args_value = f'-r {custom_world_path}'
    
    # Include Gazebo Sim launch
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(gz_sim_launch_file),
        launch_arguments={
            'gz_args': gz_args_value, 
            'use_sim_time': use_sim_time,
            'on_exit_shutdown': 'True'
        }.items()
    )
    
    # ============== SPAWN ROBOT IN GAZEBO ================ #

    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'justina',
            '-topic', 'robot_description',
            '-x', LaunchConfiguration('x'),
            '-y', LaunchConfiguration('y'),
            '-z', LaunchConfiguration('z'),
            '-Y', LaunchConfiguration('yaw'),
        ],
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )
    
    # ============== ROS-GZ BRIDGE ================ #
    
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            # Command velocity bridge (ROS -> Gazebo)
            '/cmd_vel@geometry_msgs/msg/Twist]ignition.msgs.Twist',
            # Odometry bridge (Gazebo -> ROS)
            '/odom@nav_msgs/msg/Odometry[ignition.msgs.Odometry',
            # IMU bridge (Gazebo -> ROS)
            '/imu@sensor_msgs/msg/Imu[ignition.msgs.IMU',
            # Joint states bridge (Gazebo -> ROS)
            '/joint_states@sensor_msgs/msg/JointState[ignition.msgs.Model'
        ],
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )

    # ============== BUILD LAUNCH DESCRIPTION ================ #
    # Order is critical: environment must be set first
    
    ld = LaunchDescription()
    
    # 1. SET ENVIRONMENT VARIABLES FIRST (CRITICAL FOR TEXTURES)
    for action in env_setup:
        ld.add_action(action)
    
    # 2. ADD LAUNCH ARGUMENTS
    ld.add_action(use_sim_time_declare)
    ld.add_action(x_pos)
    ld.add_action(y_pos)
    ld.add_action(z_pos)
    ld.add_action(yaw)
    
    # 3. START GAZEBO SIMULATION
    ld.add_action(gazebo_launch)
    
    # 4. START ROBOT DESCRIPTION NODES
    ld.add_action(robot_state_publisher_node)
    ld.add_action(joint_state_publisher_node)
    
    # 5. SPAWN ROBOT AFTER 3 SECONDS (let Gazebo initialize)
    ld.add_action(TimerAction(
        period=3.0,
        actions=[spawn_entity]
    ))
    
    # 6. START ROS-GAZEBO BRIDGE
    ld.add_action(bridge)
    
    return ld