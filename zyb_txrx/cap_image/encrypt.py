import cv2
import os
import glob
import numpy as np

def get_latest_image():
    """Finds the most recent image file in the current directory."""
    images = sorted(glob.glob("*.png"), key=os.path.getmtime, reverse=True)
    return images[0] if images else None

def quantize_image(image):
    # Quantize the image with 2 bits for red, 3 bits for green, and 2 bits for blue
    r_factor = 2 ** (8 - 2)  # 2 bits for red
    g_factor = 2 ** (8 - 3)  # 3 bits for green
    b_factor = 2 ** (8 - 2)  # 2 bits for blue

    # Split channels
    b, g, r = cv2.split(image)

    # Quantize each channel separately
    r = (r // r_factor) * r_factor
    g = (g // g_factor) * g_factor
    b = (b // b_factor) * b_factor

    # Merge channels back
    quantized = cv2.merge((b, g, r))
    return quantized

def apply_sharpening(image):
    # Define a simple sharpening kernel
    sharpening_kernel = np.array([[0, -1, 0],
                                  [-1, 5, -1],
                                  [0, -1, 0]])
    # Apply the kernel to the image
    sharpened_image = cv2.filter2D(image, -1, sharpening_kernel)
    return sharpened_image

def apply_edge_overlay(image):
    # Convert to grayscale and detect edges using Canny
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 50, 150)

    # Create a mask where edges are white
    edge_mask = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)

    # Overlay edges on the original image
    enhanced_image = cv2.addWeighted(image, 0.8, edge_mask, 0.2, 0)
    return enhanced_image

def encrypt():
    latest_image = get_latest_image()

    if latest_image is None:
        print("Error: No image files found.")
        exit()

    # Read the image in color
    image = cv2.imread(latest_image, cv2.IMREAD_COLOR)

    # Resize while maintaining aspect ratio with a max of 250x250
    w_new, h_new = 250, 250
    image_resized = cv2.resize(image, (w_new, h_new), interpolation=cv2.INTER_AREA)

    # Apply quantization
    quantized_image = quantize_image(image_resized)

    # Apply sharpening
    sharpened_image = apply_sharpening(quantized_image)

    # Apply edge enhancement
    enhanced_image = apply_edge_overlay(sharpened_image)

    # Save to "image.zyb" with newline-separated pixel values (RGB)
    with open("image.zyb", "w") as f:
        for row in enhanced_image:
            for pixel in row:
                f.write(f"{pixel[0]},{pixel[1]},{pixel[2]}\n")

    print(f"Processed image saved as image.zyb from {latest_image}")

if __name__ == "__main__":
    encrypt()

