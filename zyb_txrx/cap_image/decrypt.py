import sys
import numpy as np
import cv2

def decode_zyb(zyb_path, width=250):
    """
    Decodes a .zyb file (which contains per-pixel 7-bit values, one per line)
    into an OpenCV image (NumPy array in BGR format), given a fixed width 
    and an automatically computed height.
    
    :param zyb_path: Path to .zyb file
    :param width:    Image width in pixels (default=250)
    :return:         Decoded image (NumPy array, BGR)
    """
    # Read all lines (each line is an integer 0–127)
    with open(zyb_path, "r") as f:
        lines = [line.strip() for line in f.readlines()]

    # Convert text lines to integers
    data = [int(x) for x in lines]

    # Compute height based on the total number of values
    num_pixels = len(data)
    if num_pixels % width != 0:
        raise ValueError(
            f"Total pixel count ({num_pixels}) is not a multiple of width ({width})."
        )
    height = num_pixels // width

    # Create an empty BGR image (8-bit)
    img = np.zeros((height, width, 3), dtype=np.uint8)

    for i, val in enumerate(data):
        # Each val is 7 bits: R in bits 6..5, G in bits 4..2, B in bits 1..0
        r_2bits = (val >> 5) & 0b11      # 2 bits for Red
        g_3bits = (val >> 2) & 0b111    # 3 bits for Green
        b_2bits = val & 0b11           # 2 bits for Blue

        # Convert to 8-bit channels by "expanding"
        r = r_2bits * 64    # 0..3 -> 0..255 in steps of 64
        g = g_3bits * 32    # 0..7 -> 0..255 in steps of 32
        b = b_2bits * 64    # 0..3 -> 0..255 in steps of 64

        row = i // width
        col = i % width
        # OpenCV uses BGR order
        img[row, col] = [b, g, r]

    return img

def decrypt():
    zyb_file = 'image.zyb'
    # We only specify the width; the height is inferred from the file
    width = 250

    # Decode .zyb to a BGR image (NumPy array)
    img = decode_zyb(zyb_file, width=width)

    # Write the image as .png
    out_png = "image.png"
    cv2.imwrite(out_png, img)
    print(f"Decoded {zyb_file} (width={width}, height={img.shape[0]}) and saved as {out_png}")

if __name__ == "__main__":
    decrypt()

