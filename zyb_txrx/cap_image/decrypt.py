import sys
import numpy as np
import cv2

def decode_zyb(zyb_path, width, height):
    """
    Decodes a .zyb file (which contains per-pixel 7-bit values, one per line)
    into an OpenCV image (NumPy array in BGR format).
    
    :param zyb_path: Path to .zyb file
    :param width:    Image width in pixels
    :param height:   Image height in pixels
    :return:         Decoded image (NumPy array, BGR)
    """
    # Read all lines (each line is an integer 0–127)
    with open(zyb_path, "r") as f:
        lines = [line.strip() for line in f.readlines()]

    # Convert text lines to integers
    data = [int(x) for x in lines]

    # Check we have exactly width*height values
    if len(data) != width * height:
        raise ValueError(
            f"Mismatch: .zyb file has {len(data)} values, but "
            f"width*height = {width}*{height} = {width*height}."
        )

    # Create an empty BGR image (8-bit)
    img = np.zeros((height, width, 3), dtype=np.uint8)

    for i, val in enumerate(data):
        # Each val is 7 bits: R in bits 6..5, G in bits 4..2, B in bits 1..0
        # Extract them:
        r_2bits = (val >> 5) & 0b11      # 2 bits for Red
        g_3bits = (val >> 2) & 0b111    # 3 bits for Green
        b_2bits = val & 0b11           # 2 bits for Blue

        # Convert back to 8-bit channels by "expanding":
        #   R: 0..3 -> 0..255 is approximate with factor 64 (4*64=256)
        #   G: 0..7 -> 0..255 is approximate with factor 32 (8*32=256)
        #   B: 0..3 -> 0..255 is approximate with factor 64 (4*64=256)
        r = r_2bits * 64
        g = g_3bits * 32
        b = b_2bits * 64

        row = i // width
        col = i % width
        # OpenCV uses BGR order
        img[row, col] = [b, g, r]

    return img

def decrypt(width=250, height=250):
    zyb_file = 'image.zyb'

    # Decode .zyb to a BGR image (NumPy array)
    img = decode_zyb(zyb_file, width, height)

    # Write the image as .png
    out_png = "image.png"
    cv2.imwrite(out_png, img)
    print(f"Decoded {zyb_file} and saved as {out_png}")

if __name__ == "__main__":
    decrypt()

