import os
import glob
import cv2

def get_latest_image():
    """
    Finds the most recent .png file in the current directory.
    Returns the filename (str) or None if none is found.
    """
    images = sorted(glob.glob("*.png"), key=os.path.getmtime, reverse=True)
    return images[0] if images else None

def encrypt_to_zyb(image_path, output_path="image.zyb"):
    """
    Read the image at `image_path` using OpenCV, convert each pixel
    to a single 7-bit value (2 bits R, 3 bits G, 2 bits B), and save
    all pixel values (0–127) line-by-line to `output_path`.
    """
    # Read the image in BGR format
    img = cv2.imread(image_path)
    if img is None:
        print(f"Could not open image: {image_path}")
        return

    height, width, _ = img.shape
    
    with open(output_path, "w") as f:
        # Iterate through each pixel
        for row in range(height):
            for col in range(width):
                b, g, r = img[row, col]

                # Quantize each channel:
                #  - R (2 bits) => 0..3   (divide by 64 => 256/4=64)
                #  - G (3 bits) => 0..7   (divide by 32 => 256/8=32)
                #  - B (2 bits) => 0..3   (divide by 64 => 256/4=64)
                r_2bits = r // 64  # range 0..3
                g_3bits = g // 32  # range 0..7
                b_2bits = b // 64  # range 0..3

                # Combine into 7 bits: 
                #   bit 6..5 = R (2 bits)
                #   bit 4..2 = G (3 bits)
                #   bit 1..0 = B (2 bits)
                # This yields a value from 0..127
                color_7bit = (r_2bits << 5) | (g_3bits << 2) | b_2bits

                f.write(str(color_7bit) + "\n")

def encrypt():
    latest_png = get_latest_image()
    if not latest_png:
        print("No .png images found in the current directory.")
        return

    encrypt_to_zyb(latest_png, "image.zyb")
    print(f"Converted {latest_png} to 7-bit values and saved to image.zyb.")

if __name__ == "__main__":
    encrypt()

