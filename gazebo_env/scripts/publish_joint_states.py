#!/usr/bin/env python3
"""
Simple joint state publisher for simulation.
"""
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

class SimpleJointPublisher(Node):
    def __init__(self):
        super().__init__('simple_joint_publisher')
        self.pub = self.create_publisher(JointState, '/joint_states', 10)
        self.timer = self.create_timer(0.1, self.publish)
        self.joint_names = [
            'wheel0_joint', 'wheel1_joint', 'wheel2_joint',
            'head_pan', 'head_tilt'
        ]
        self.get_logger().info('Publishing joint states')
    
    def publish(self):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'world'
        msg.name = self.joint_names
        msg.position = [0.0] * len(self.joint_names)
        msg.velocity = [0.0] * len(self.joint_names)
        self.pub.publish(msg)

def main():
    rclpy.init()
    rclpy.spin(SimpleJointPublisher())
    rclpy.shutdown()

if __name__ == '__main__':
    main()