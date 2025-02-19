import cv2
import datetime

def capture_image():

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

    # Generate filename with current date and time
    filename = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"

    # Save the image
    cv2.imwrite(filename, frame)
    print(f"Image saved as {filename}")

    # Release the camera
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    capture_image()
