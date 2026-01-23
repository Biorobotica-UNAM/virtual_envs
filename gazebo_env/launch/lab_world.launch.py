#!/usr/bin/env python3
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import ExecuteProcess, SetEnvironmentVariable

def generate_launch_description():
    # Get the package install directory
    pkg_dir = get_package_share_directory('gazebo_env')
    
    # World file is in the package root
    world_path = os.path.join(pkg_dir, 'lab_2026.sdf')
    
    print("=" * 60)
    print("LAUNCHING GAZEBO SIMULATION")
    print("=" * 60)
    print(f"Package directory: {pkg_dir}")
    print(f"World file: {world_path}")
    print(f"File exists: {os.path.exists(world_path)}")
    
    # Set the resource path to the package directory
    # This is where BioRoboticsLab and other models are located
    resource_path = pkg_dir
    
    # Launch Gazebo
    return LaunchDescription([
        # Set environment variable for all processes
        SetEnvironmentVariable(
            name='GZ_SIM_RESOURCE_PATH',
            value=resource_path
        ),
        SetEnvironmentVariable(
            name='IGN_GAZEBO_RESOURCE_PATH',
            value=resource_path
        ),
        
        ExecuteProcess(
            cmd=[
                'gz', 'sim',
                '-v', '2',  # Reduced verbosity
                world_path
            ],
            output='screen',
            # Ensure environment is passed
            additional_env={
                'GZ_SIM_RESOURCE_PATH': resource_path,
                'IGN_GAZEBO_RESOURCE_PATH': resource_path,
            }
        )
    ])