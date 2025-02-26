import cv2
import numpy as np
import datetime

def capture_image(capture_width=250, capture_height=250, sharpen=False):
    capture_width = max(250, capture_width)
    capture_height = max(250, capture_height)

    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    ret, frame = cap.read()

    if not ret:
        print("Error: Could not read frame.")
        cap.release()
        exit()

    frame_height, frame_width, _ = frame.shape

    start_x = max(0, (frame_width - capture_width) // 2)
    start_y = max(0, (frame_height - capture_height) // 2)
    end_x = start_x + capture_width
    end_y = start_y + capture_height

    cropped_frame = frame[start_y:end_y, start_x:end_x]
    if sharpen:
        sharpening_kernel = np.array([[-1, -1, -1], 
                                      [-1,  9, -1], 
                                      [-1, -1, -1]])
        cropped_frame = cv2.filter2D(cropped_frame, -1, sharpening_kernel)

    filename = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"

    cv2.imwrite(filename, cropped_frame)
    print(f"Image saved as {filename}")

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    capture_image(sharpen=True)
