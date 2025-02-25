import cv2
import numpy as np
import datetime

def capture_image(capture_width=250, capture_height=250, sharpen=False):
    # Ensure minimum size is 250x250
    capture_width = max(250, capture_width)
    capture_height = max(250, capture_height)

    # Open the first available camera
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    # Capture a frame
    ret, frame = cap.read()

    if not ret:
        print("Error: Could not read frame.")
        cap.release()
        exit()

    # Get the dimensions of the frame
    frame_height, frame_width, _ = frame.shape

    # Calculate the center crop coordinates
    start_x = max(0, (frame_width - capture_width) // 2)
    start_y = max(0, (frame_height - capture_height) // 2)
    end_x = start_x + capture_width
    end_y = start_y + capture_height

    # Crop the frame to the desired size
    cropped_frame = frame[start_y:end_y, start_x:end_x]

    if sharpen:
        sharpening_kernel = np.array([[-1, -1, -1], 
                                      [-1,  9, -1], 
                                      [-1, -1, -1]])
        # Apply the sharpening filter
        cropped_frame = cv2.filter2D(cropped_frame, -1, sharpening_kernel)

    # Generate filename with current date and time
    filename = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"

    # Save the cropped image
    cv2.imwrite(filename, cropped_frame)
    print(f"Image saved as {filename}")

    # Release the camera
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    capture_image(sharpen=True)
