#!/usr/bin/env python3
import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, SetEnvironmentVariable, TimerAction
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue   # <-- IMPORTANT
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # ---------- ENVIRONMENT (same as original, critical for textures) ----------
    justina_desc_path = get_package_share_directory('justina_description')
    gazebo_env_pkg = get_package_share_directory('gazebo_env')
    webots_env_share = get_package_share_directory('webots_env')
    models_path = os.path.join(webots_env_share, 'models', 'BioRoboticsLab')

    resource_paths = [
        '/opt/ros/jazzy/share',
        justina_desc_path,
        os.path.join(justina_desc_path, 'meshes', 'head'),
        os.path.dirname(justina_desc_path),
        models_path,                                # Webots models directory
    ]
    resource_paths = [p for p in resource_paths if p]
    gz_resource_path = os.path.pathsep.join(resource_paths)

    env_setup = [
        SetEnvironmentVariable(name='GZ_SIM_RESOURCE_PATH', value=gz_resource_path),
        SetEnvironmentVariable(name='GAZEBO_RESOURCE_PATH',
                               value=os.path.join(justina_desc_path, 'meshes', 'head')),
        SetEnvironmentVariable(name='GAZEBO_MODEL_PATH',
                               value=os.path.join(justina_desc_path, 'meshes', 'head')),
    ]

    # ---------- LAUNCH ARGUMENTS ----------
    use_sim_time_declare = DeclareLaunchArgument(
        'use_sim_time', default_value='true',
        description='Use simulation (Gazebo) clock if true')
    use_sim_time = LaunchConfiguration('use_sim_time')

    x_pos = DeclareLaunchArgument('x', default_value='0.0')
    y_pos = DeclareLaunchArgument('y', default_value='1.0')
    z_pos = DeclareLaunchArgument('z', default_value='0.1')
    yaw   = DeclareLaunchArgument('yaw', default_value='0.0')

    # ---------- XACRO PROCESSING ----------
    # Path to the new robot's xacro file
    xacro_file = PathJoinSubstitution([
        FindPackageShare('justina_description'),
        'urdf', 'gazebo_blender', 'justina_robot.urdf'
    ])

    # Outer single quotes, inner escaped double quotes.
    robot_description_command = Command([
        "bash -c 'xacro ", xacro_file,
        " | sed \"s|<mesh filename=\\\"meshes/|<mesh filename=\\\"package://justina_description/meshes/|g\"'"
    ])

    robot_description_param = ParameterValue(robot_description_command, value_type=str)

    # ---------- NODES ----------
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[{
            'use_sim_time': use_sim_time,
            'robot_description': robot_description_param   # <-- now a proper string parameter
        }]
    )

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

    # ---------- GAZEBO WITH THE SAME CUSTOM WORLD ----------
    gz_sim_launch = os.path.join(
        get_package_share_directory('ros_gz_sim'),
        'launch', 'gz_sim.launch.py'
    )
    custom_world = os.path.join(
        gazebo_env_pkg, 'worlds', 'lab_2026.sdf'
    )
    gz_args_value = f'-r {custom_world}'

    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(gz_sim_launch),
        launch_arguments={
            'gz_args': gz_args_value,
            'use_sim_time': use_sim_time,
            'on_exit_shutdown': 'True'
        }.items()
    )

    # ---------- SPAWN THE NEW ROBOT (unique name) ----------
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'test_robot',          # distinct from original "justina"
            '-topic', 'robot_description',
            '-x', LaunchConfiguration('x'),
            '-y', LaunchConfiguration('y'),
            '-z', LaunchConfiguration('z'),
            '-Y', LaunchConfiguration('yaw'),
        ],
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )

    # ---------- ROS‑GZ BRIDGE (same topics) ----------
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/cmd_vel@geometry_msgs/msg/Twist]ignition.msgs.Twist',
            '/odom@nav_msgs/msg/Odometry[ignition.msgs.Odometry',
            '/imu@sensor_msgs/msg/Imu[ignition.msgs.IMU',
            '/joint_states@sensor_msgs/msg/JointState[ignition.msgs.Model'
        ],
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )

    # ---------- BUILD LAUNCH DESCRIPTION ----------
    ld = LaunchDescription()

    # 1. Environment variables (must be first)
    for action in env_setup:
        ld.add_action(action)

    # 2. Launch arguments
    ld.add_action(use_sim_time_declare)
    ld.add_action(x_pos)
    ld.add_action(y_pos)
    ld.add_action(z_pos)
    ld.add_action(yaw)

    # 3. Gazebo simulation
    ld.add_action(gazebo_launch)

    # 4. Robot description nodes
    ld.add_action(robot_state_publisher_node)
    ld.add_action(joint_state_publisher_node)

    # 5. Spawn robot after 3 seconds
    ld.add_action(TimerAction(period=3.0, actions=[spawn_entity]))

    # 6. Bridge
    ld.add_action(bridge)

    return ld