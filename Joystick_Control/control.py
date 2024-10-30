import pygame
import pyautogui
import time

# Initialize pygame
pygame.init()

# Initialize the joystick
if pygame.joystick.get_count() == 0:
    print("No joystick connected!")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Joystick Name: {joystick.get_name()}")

# Dead zone threshold
dead_zone = 0.2  # Adjust this value to avoid small noise

# Set a small delay to avoid flooding the input
time.sleep(2)

# Initialize last X and Y for comparison
last_x, last_y = None, None

try:
    while True:
        pygame.event.pump()

        # Read X and Y axis data and scale by 100 (range will be -100 to 100)
        x_axis = joystick.get_axis(0) * 100  # Keep the sign (-100 to 100)
        y_axis = joystick.get_axis(1) * -100  # Keep the sign (-100 to 100)

        # Round values to integers for more readable output
        x_axis = round(x_axis)
        y_axis = round(y_axis)

        # Only type if values changed significantly to avoid repeated numbers
        data = f"{x_axis} {y_axis}"

            # Simulate typing the data with a faster speed
        pyautogui.typewrite(data + '\n', interval=0.0005)  # Adjusted interval for faster typing
        print(f"Typed: {data}")

            # Update last X and Y
        last_x, last_y = x_axis, y_axis

        # Small delay to avoid flooding
        time.sleep(0.01)

except KeyboardInterrupt:
    print("\nExiting...")
finally:
    pygame.quit()
script