#!/usr/bin/env python3
"""
Generate URDF with absolute file:// paths AND Gazebo control plugin.
"""
import os
import subprocess
import sys
from ament_index_python.packages import get_package_share_directory

def main():
    # Get package directories
    justina_desc_pkg = get_package_share_directory('justina_description')
    
    # Generate original URDF
    xacro_path = os.path.join(justina_desc_pkg, 'urdf', 'justina', 'justina.xacro')
    cmd = ['xacro', xacro_path, 'use_sim_time:=true']
    
    try:
        original_urdf = subprocess.check_output(cmd, text=True)
        
        # Replace model:// and package:// paths with file://
        modified_urdf = original_urdf
        
        # Replace package://justina_description
        modified_urdf = modified_urdf.replace(
            'package://justina_description/', 
            f'file://{justina_desc_pkg}/'
        )
        
        # Replace model://justina_description  
        modified_urdf = modified_urdf.replace(
            'model://justina_description/', 
            f'file://{justina_desc_pkg}/'
        )
        
        # ====== ADD GAZEBO CONTROL PLUGIN ======
        # Find where to insert the plugin (before closing </robot> tag)
        if '</robot>' in modified_urdf:
            # Add diff drive plugin for Robotino (simplified for 3 omni wheels)
            gazebo_plugin = """
  <!-- Gazebo Control Plugin -->
  <gazebo>
    <plugin filename="libgz-sim-diff-drive-system.so" name="gz::sim::systems::DiffDrive">
      <left_joint>wheel0_joint</left_joint>
      <right_joint>wheel1_joint</right_joint>
      <wheel_separation>0.5</wheel_separation>
      <wheel_radius>0.1</wheel_radius>
      <odom_frame>odom</odom_frame>
      <robot_base_frame>base_footprint</robot_base_frame>
      <odom_publish_frequency>30</odom_publish_frequency>
      <topic>cmd_vel</topic>
    </plugin>
    
    <!-- Joint State Publisher -->
    <plugin filename="libgz-sim-joint-state-publisher-system.so" name="gz::sim::systems::JointStatePublisher">
      <joint_name>wheel0_joint</joint_name>
      <joint_name>wheel1_joint</joint_name>
      <joint_name>wheel2_joint</joint_name>
      <update_rate>50</update_rate>
    </plugin>
  </gazebo>
"""
            modified_urdf = modified_urdf.replace('</robot>', gazebo_plugin + '\n</robot>')
        
        # Print the modified URDF to stdout
        print(modified_urdf)
        return 0
        
    except Exception as e:
        print(f"Error generating URDF: {e}", file=sys.stderr)
        return 1

if __name__ == '__main__':
    sys.exit(main())