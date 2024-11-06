import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32  # Import the correct message type (Int32)

class STM32Subscriber(Node):
    def __init__(self):
        super().__init__('stm32_subscriber')  # Create a node called 'stm32_subscriber'
        # Create a subscriber to the topic that the STM32 is publishing
        self.subscription = self.create_subscription(
            Int32,  # Correct message type (Int32 instead of String)
            'stm32_topic',  # Topic name must match the STM32 topic name
            self.listener_callback,
            10  # Queue size
        )
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        # Print the received message
        self.get_logger().info('Received message: "%d"' % msg.data)  # Change %s to %d for integers

def main(args=None):
    rclpy.init(args=args)  # Initialize the ROS 2 Python client library
    node = STM32Subscriber()  # Create the subscriber node

    # Spin the node to keep it alive and listening for messages
    rclpy.spin(node)

    # Cleanup after spinning (not really necessary here, as spinning handles cleanup)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
