import os
import glob
import cv2

def get_latest_image():
    images = sorted(glob.glob("*.png"), key=os.path.getmtime, reverse=True)
    return images[0] if images else None

def encrypt_to_zyb(image_path, output_path="image.zyb"):
    img = cv2.imread(image_path)
    if img is None:
        print(f"Could not open image: {image_path}")
        return

    height, width, _ = img.shape
    
    with open(output_path, "w") as f:
        for row in range(height):
            for col in range(width):
                b, g, r = img[row, col]
                r_2bits = r // 64  # range 0..3
                g_3bits = g // 32  # range 0..7
                b_2bits = b // 64  # range 0..3
                color_7bit = (r_2bits << 5) | (g_3bits << 2) | b_2bits | 1

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

