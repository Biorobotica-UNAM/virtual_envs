from controller import Robot, Motor
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

TIME_STEP = 32

class JustinaController(Node):
    def __init__(self):
        super().__init__('justina_controller')
        self.robot = Robot()

        # Replace with actual motor names in the PROTO
        self.motors = [
            self.robot.getDevice('wheel0_joint'),
            self.robot.getDevice('wheel1_joint'),
            self.robot.getDevice('wheel2_joint'),
            self.robot.getDevice('wheel3_joint')
        ]
        for m in self.motors:
            m.setPosition(float('inf'))  # velocity control mode
            m.setVelocity(0.0)

        # ROS 2 subscription
        self.subscription = self.create_subscription(
            Twist,
            '/cmd_vel',
            self.cmd_vel_callback,
            10
        )

    def cmd_vel_callback(self, msg: Twist):
        # Simple differential drive conversion
        left_speed = msg.linear.x - msg.angular.z
        right_speed = msg.linear.x + msg.angular.z
        self.motors[0].setVelocity(left_speed)
        self.motors[1].setVelocity(left_speed)
        self.motors[2].setVelocity(right_speed)
        self.motors[3].setVelocity(right_speed)

    def run(self):
        while self.robot.step(TIME_STEP) != -1:
            rclpy.spin_once(self, timeout_sec=0)

def main(args=None):
    rclpy.init(args=args)
    controller = JustinaController()
    controller.run()
    controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()