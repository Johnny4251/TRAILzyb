import cv2
import numpy as np
import os

def load_zyb_file(filename):
    """Loads the .zyb file and converts it into a grayscale image."""
    try:
        with open(filename, "r") as f:
            pixel_values = [int(line.strip()) for line in f]
    except FileNotFoundError:
        print("Error: image.zyb not found.")
        exit()
    
    # Estimate the original image dimensions (assuming it was square or near-square)
    length = len(pixel_values)
    side = int(np.sqrt(length))  # Approximate side length of square image
    
    if side * side != length:
        print("Warning: Pixel count does not form a perfect square. Image may be distorted.")
    
    # Reshape pixel values into a 2D NumPy array
    image_array = np.array(pixel_values[:side * side], dtype=np.uint8).reshape((side, side))
    
    return image_array

def decrypt():
    # Load the encrypted .zyb file
    image_array = load_zyb_file("image.zyb")

    # Save as a PNG file
    output_filename = "image.png"
    cv2.imwrite(output_filename, image_array)
    print(f"Decrypted image saved as {output_filename}")

if __name__ == "__main__":
    decrypt
