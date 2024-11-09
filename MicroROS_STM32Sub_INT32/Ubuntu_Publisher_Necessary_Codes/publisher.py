import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

class Int32Publisher(Node):
    def __init__(self):
        super().__init__('int32_publisher')
        # Create a publisher that will send Int32 messages to the "int32_topic"
        self.publisher_ = self.create_publisher(Int32, 'stm32_topic', 10)
        self.timer = self.create_timer(1.0, self.timer_callback)  # Publish every 1 second
        self.count = 0

    def timer_callback(self):
        # Create a new Int32 message
        msg = Int32()
        msg.data = self.count
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: {msg.data}')
        self.count += 1

def main(args=None):
    rclpy.init(args=args)

    # Create the node
    node = Int32Publisher()

    # Spin the node so it can process callbacks
    rclpy.spin(node)

    # Destroy the node (not needed in this example, but a good practice)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
