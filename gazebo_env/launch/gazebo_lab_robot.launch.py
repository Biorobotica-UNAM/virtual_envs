#!/usr/bin/env python3
"""
Launch file for Justina robot in Gazebo simulation.
Loads lab_2026.sdf world and spawns Justina robot at (0, 1, 0).
"""
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    ExecuteProcess,
    SetEnvironmentVariable,
    LogInfo,
    TimerAction,
    RegisterEventHandler,
)
from launch.event_handlers import OnProcessStart
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node

def generate_launch_description():
    # Package directories
    gazebo_env_pkg = get_package_share_directory('gazebo_env')
    justina_desc_pkg = get_package_share_directory('justina_description')
    
    # Paths
    world_path = os.path.join(gazebo_env_pkg, 'lab_2026.sdf')
    
    # Use simulation time
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    
    # Get the install directory root
    install_root = os.path.dirname(os.path.dirname(justina_desc_pkg))  # Go up two levels from share/justina_description
    
    # Create proper resource paths
    resource_paths = [
        gazebo_env_pkg,  # For lab_2026.sdf and BioRoboticsLab models
        install_root,  # The install directory root
        justina_desc_pkg,  # The share/justina_description directory
    ]
    
    resource_path = ':'.join(resource_paths)
    
    # Spawn coordinates (x, y, z, yaw)
    spawn_x = '0.0'
    spawn_y = '1.0'
    spawn_z = '0.0'
    spawn_yaw = '0.0'
    
    # ====== DEBUG: Verify paths ======
    print("=" * 60)
    print("DEBUG - MESH FILE LOCATIONS:")
    print("=" * 60)
    
    # Test specific files
    test_cases = [
        ("Robotino3_SplitMain_NoCamera.dae", 
         os.path.join(justina_desc_pkg, 'model', 'robotino', 'Robotino3_SplitMain_NoCamera.dae')),
        ("justina_chest.dae",
         os.path.join(justina_desc_pkg, 'meshes', 'justina', 'torso', 'justina_chest.dae')),
        ("hokuyo.dae",
         os.path.join(justina_desc_pkg, 'meshes', 'robotino', 'sensors', 'hokuyo.dae')),
    ]
    
    for name, path in test_cases:
        if os.path.exists(path):
            print(f"✓ {name}: {path}")
            # Also check if it's readable
            if os.access(path, os.R_OK):
                print(f"  File is readable")
            else:
                print(f"  WARNING: File exists but is not readable!")
        else:
            print(f"✗ {name}: NOT FOUND at {path}")
            # Try to find it
            import subprocess
            result = subprocess.run(['find', install_root, '-name', name, '-type', 'f'], 
                                  capture_output=True, text=True)
            if result.stdout:
                print(f"  Found at: {result.stdout.strip()}")
    
    print("=" * 60)
    
    # 1. Launch Gazebo with the world
    gazebo_process = ExecuteProcess(
        cmd=[
            'gz', 'sim',
            '-v', '3',  # Increased verbosity for debugging
            world_path,
            '--force-version', '8',  # Force Gazebo Sim version 8
        ],
        output='screen',
        additional_env={
            'GZ_SIM_RESOURCE_PATH': resource_path,
            'IGN_GAZEBO_RESOURCE_PATH': resource_path,
            'GAZEBO_MODEL_PATH': resource_path,
            'GZ_SIM_SYSTEM_PLUGIN_PATH': '',  # Clear plugin path to avoid errors
        }
    )
    
    # 2. Create a modified URDF with file:// paths instead of model://
    # First, let's create a temporary URDF with absolute paths
    import tempfile
    import subprocess
    
    # Generate the original URDF
    urdf_command = [
        'xacro',
        os.path.join(justina_desc_pkg, 'urdf', 'justina', 'justina.xacro'),
        'use_sim_time:=true'
    ]
    
    original_urdf = subprocess.check_output(urdf_command, text=True)
    
    # Replace model:// with file:// paths
    modified_urdf = original_urdf
    
    # Common replacements
    replacements = {
        'model://justina_description/': f'file://{justina_desc_pkg}/',
        'package://justina_description/': f'file://{justina_desc_pkg}/',
    }
    
    for old, new in replacements.items():
        modified_urdf = modified_urdf.replace(old, new)
    
    # Create a temporary file with the modified URDF
    with tempfile.NamedTemporaryFile(mode='w', suffix='.urdf', delete=False) as f:
        f.write(modified_urdf)
        temp_urdf_path = f.name
    
    print(f"Created temporary URDF at: {temp_urdf_path}")
    
    # 3. Robot State Publisher with modified URDF
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'robot_description': modified_urdf,  # Use the modified URDF string
            'publish_frequency': 50.0,
        }],
        remappings=[
            ('/joint_states', '/hardware/joint_states')
        ]
    )
    
    # 4. Joint State Publisher
    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}],
        remappings=[
            ('/joint_states', '/hardware/joint_states')
        ]
    )
    
    # 5. Spawn robot in Gazebo at (0, 1, 0)
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-name', 'justina',
            '-file', temp_urdf_path,  # Use the temporary URDF file
            '-x', spawn_x,
            '-y', spawn_y,
            '-z', spawn_z,
            '-Y', spawn_yaw
        ],
        output='screen'
    )
    
    # 6. Simple clock bridge
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='gz_bridge',
        arguments=[
            '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',
        ],
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )
    
    # 7. Simple joint state publisher
    simple_joint_publisher = Node(
        package='gazebo_env',  # Assuming this script is in gazebo_env package
        executable='publish_joint_states.py',
        name='simple_joint_publisher',
        output='screen',
        parameters=[{'use_sim_time': use_sim_time}]
    )

    return LaunchDescription([
        # Set environment variables
        SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', resource_path),
        SetEnvironmentVariable('IGN_GAZEBO_RESOURCE_PATH', resource_path),
        SetEnvironmentVariable('GAZEBO_MODEL_PATH', resource_path),
        
        # Log info
        LogInfo(msg='=' * 60),
        LogInfo(msg='LAUNCHING JUSTINA SIMULATION'),
        LogInfo(msg='=' * 60),
        LogInfo(msg=f'World file: {world_path}'),
        LogInfo(msg=f'Robot spawn: ({spawn_x}, {spawn_y}, {spawn_z}, {spawn_yaw})'),
        LogInfo(msg=f'Resource path: {resource_path}'),
        LogInfo(msg='=' * 60),
        
        # Start Gazebo
        gazebo_process,
        
        # After Gazebo starts, spawn robot
        RegisterEventHandler(
            OnProcessStart(
                target_action=gazebo_process,
                on_start=[
                    TimerAction(
                        period=4.0,  # Wait 4 seconds for world to load
                        actions=[
                            LogInfo(msg='World loaded, spawning robot...'),
                            robot_state_publisher,
                            joint_state_publisher,
                            TimerAction(
                                period=1.0,
                                actions=[
                                    spawn_entity,
                                ]
                            )
                        ]
                    )
                ]
            )
        ),
        
        # After robot is spawned, start bridge and joint publisher
        RegisterEventHandler(
            OnProcessStart(
                target_action=spawn_entity,
                on_start=[
                    TimerAction(
                        period=2.0,
                        actions=[
                            LogInfo(msg='Robot spawned, starting services...'),
                            bridge,
                            simple_joint_publisher,
                            LogInfo(msg='=' * 60),
                            LogInfo(msg='SIMULATION READY!'),
                            LogInfo(msg='To check joint states: ros2 topic echo /hardware/joint_states'),
                            LogInfo(msg='To control robot: ros2 run base robotino_teleop.py'),
                            LogInfo(msg='=' * 60),
                        ]
                    )
                ]
            )
        ),
    ])