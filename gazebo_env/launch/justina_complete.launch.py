#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction
import os
import subprocess
import tempfile

def generate_launch_description():
    # Get package directories
    from ament_index_python.packages import get_package_share_directory
    gazebo_env_pkg = get_package_share_directory('gazebo_env')
    justina_desc_pkg = get_package_share_directory('justina_description')
    
    # Generate URDF
    print("Generating URDF for Justina...")
    xacro_file = os.path.join(justina_desc_pkg, 'urdf', 'justina', 'justina_gazebo.xacro')
    
    # Set environment for xacro
    env = os.environ.copy()
    env['ROS_PACKAGE_PATH'] = f"{justina_desc_pkg}:{env.get('ROS_PACKAGE_PATH', '')}"
    
    try:
        urdf_content = subprocess.check_output(
            ['xacro', xacro_file],
            cwd=justina_desc_pkg,
            env=env,
            text=True
        )
        
        # Fix paths from package:// to absolute
        urdf_content = urdf_content.replace('package://justina_description/', justina_desc_pkg + '/')
        
        # Save to temporary file
        with tempfile.NamedTemporaryFile(mode='w', suffix='.urdf', delete=False) as f:
            f.write(urdf_content)
            urdf_path = f.name
        
        print(f"URDF generated: {urdf_path}")
        
    except Exception as e:
        print(f"Error generating URDF: {e}")
        # Create a simple fallback URDF
        urdf_path = os.path.join(tempfile.gettempdir(), 'justina_fallback.urdf')
        with open(urdf_path, 'w') as f:
            f.write('''<?xml version="1.0"?>
<robot name="justina_fallback">
  <link name="base_link">
    <visual>
      <geometry>
        <cylinder length="0.1" radius="0.5"/>
      </geometry>
      <material name="blue">
        <color rgba="0 0 0.8 1"/>
      </material>
    </visual>
  </link>
</robot>''')
        print(f"Using fallback URDF: {urdf_path}")
    
    ld = LaunchDescription()
    
    # Start Gazebo with environment
    env = os.environ.copy()
    env['GZ_SIM_RESOURCE_PATH'] = f'{gazebo_env_pkg}:{justina_desc_pkg}'
    env['QT_LOGGING_RULES'] = '*.debug=false;qt.qpa.*=false'
    
    world_path = os.path.join(gazebo_env_pkg, 'lab_2026.sdf')
    gazebo_process = ExecuteProcess(
        cmd=['gz', 'sim', '-v', '2', '--render-engine', 'ogre2', world_path],
        output='screen',
        env=env
    )
    ld.add_action(gazebo_process)
    
    # Spawn robot after delay
    spawn_process = ExecuteProcess(
        cmd=['ros2', 'run', 'ros_gz_sim', 'create', '-file', urdf_path,
             '-name', 'justina', '-x', '0', '-y', '1', '-z', '0.05'],
        output='screen'
    )
    
    # Wait 3 seconds then spawn
    ld.add_action(TimerAction(
        period=3.0,
        actions=[spawn_process]
    ))
    
    return ld

if __name__ == '__main__':
    generate_launch_description()