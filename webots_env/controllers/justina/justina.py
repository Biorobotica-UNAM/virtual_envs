from controller import Robot
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, TransformStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import JointState
from tf2_ros import TransformBroadcaster
import math

class RobotController(Node):
    """
    Template ROS2 controller for a differential-drive robot
    that can be extended with sensors, arms, or other actuators.
    """

    def __init__(self, robot: Robot):
        super().__init__('robot_controller')

        # Webots Robot
        self.robot = robot
        self.time_step = int(robot.getBasicTimeStep())

        # Robot geometry
        self.wheel_radius = 0.05
        self.wheel_separation = 0.22  # distance between wheels

        # Motors
        self.left_motor = robot.getDevice('left_wheel')
        self.right_motor = robot.getDevice('right_wheel')
        self.left_motor.setPosition(float('inf'))
        self.right_motor.setPosition(float('inf'))
        self.left_motor.setVelocity(0.0)
        self.right_motor.setVelocity(0.0)

        # State
        self.v = 0.0
        self.w = 0.0
        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0
        self.last_time = self.get_clock().now()

        # ROS 2 interfaces
        self.create_subscription(Twist, '/cmd_vel', self.cmd_callback, 10)
        self.odom_pub = self.create_publisher(Odometry, '/odom', 10)
        self.joint_pub = self.create_publisher(JointState, '/joint_states', 10)
        self.tf_broadcaster = TransformBroadcaster(self)

        # Add any additional actuated joints here
        # Format: {joint_name: device_object}
        self.actuated_joints = {}

    def cmd_callback(self, msg: Twist):
        self.v = msg.linear.x
        self.w = msg.angular.z

    def step(self) -> bool:
        """Perform a simulation step and publish ROS 2 topics/TFs."""
        if self.robot.step(self.time_step) == -1:
            return False

        # Compute dt
        now = self.get_clock().now()
        dt = (now - self.last_time).nanoseconds * 1e-9
        self.last_time = now

        # Compute wheel velocities
        v_left = self.v - self.w * self.wheel_separation / 2.0
        v_right = self.v + self.w * self.wheel_separation / 2.0
        self.left_motor.setVelocity(v_left / self.wheel_radius)
        self.right_motor.setVelocity(v_right / self.wheel_radius)

        # Integrate robot pose
        dx = self.v * math.cos(self.theta) * dt
        dy = self.v * math.sin(self.theta) * dt
        dtheta = self.w * dt
        self.x += dx
        self.y += dy
        self.theta += dtheta

        # Publish ROS 2 messages
        self.publish_odometry(now)
        self.publish_joint_states(now)

        rclpy.spin_once(self, timeout_sec=0.0)
        return True

    def publish_odometry(self, now):
        """Publish odometry and base_link TFs."""
        # Odometry message
        odom = Odometry()
        odom.header.stamp = now.to_msg()
        odom.header.frame_id = "odom"
        odom.child_frame_id = "base_footprint"
        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        qz = math.sin(self.theta / 2.0)
        qw = math.cos(self.theta / 2.0)
        odom.pose.pose.orientation.z = qz
        odom.pose.pose.orientation.w = qw
        self.odom_pub.publish(odom)

        # TF: odom -> base_footprint
        t_odom = TransformStamped()
        t_odom.header.stamp = now.to_msg()
        t_odom.header.frame_id = "odom"
        t_odom.child_frame_id = "base_footprint"
        t_odom.transform.translation.x = self.x
        t_odom.transform.translation.y = self.y
        t_odom.transform.rotation.z = qz
        t_odom.transform.rotation.w = qw
        self.tf_broadcaster.sendTransform(t_odom)

        # TF: base_footprint -> base_link
        t_base = TransformStamped()
        t_base.header.stamp = now.to_msg()
        t_base.header.frame_id = "base_footprint"
        t_base.child_frame_id = "base_link"
        t_base.transform.translation.z = 0.06  # robot center height
        t_base.transform.rotation.w = 1.0
        self.tf_broadcaster.sendTransform(t_base)

        # Optional: publish additional TFs for sensors/links here

    def publish_joint_states(self, now):
        """Publish joint states for actuated joints."""
        js = JointState()
        js.header.stamp = now.to_msg()
        # Always include wheels
        js.name = ['left_wheel_joint', 'right_wheel_joint']
        js.position = [self.left_motor.getTargetPosition(), self.right_motor.getTargetPosition()]

        # Append any additional actuated joints
        for joint_name, device in self.actuated_joints.items():
            js.name.append(joint_name)
            js.position.append(device.getTargetPosition() if device else 0.0)

        self.joint_pub.publish(js)

    def add_joint(self, joint_name: str, device):
        """Register an additional actuated joint."""
        self.actuated_joints[joint_name] = device

def main():
    rclpy.init()
    robot = Robot()
    controller = RobotController(robot)

    # Example: add actuated joints (arms, torso, etc.)
    # arm_motor = robot.getDevice('arm_joint')
    # controller.add_joint('arm_joint', arm_motor)

    while controller.step():
        pass

    controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()