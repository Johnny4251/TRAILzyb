import cv2
import os
import glob
import numpy as np

def get_latest_image():
    """Finds the most recent image file in the current directory."""
    images = sorted(glob.glob("*.png"), key=os.path.getmtime, reverse=True)
    return images[0] if images else None

def encrypt():
    latest_image = get_latest_image()

    if latest_image is None:
        print("Error: No image files found.")
        exit()

    # Read the image in grayscale
    image = cv2.imread(latest_image, cv2.IMREAD_GRAYSCALE)

    # Resize while maintaining aspect ratio with a max of 250x250
    h, w = image.shape
    w_new, h_new = 250,250
    image_resized = cv2.resize(image, (w_new, h_new), interpolation=cv2.INTER_AREA)

    # Clip to max 128-bit grayscale values
    image_resized = np.clip(image_resized, 0, 128).astype(np.uint8)

    # Save to "image.zyb" with newline-separated pixel values
    with open("image.zyb", "w") as f:
        for row in image_resized:
            for pixel in row:
                f.write(f"{pixel}\n")

    print(f"Processed image saved as image.zyb from {latest_image}")

if __name__ == "__main__":
    encrypt()
