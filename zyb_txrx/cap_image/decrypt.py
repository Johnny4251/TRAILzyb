import sys
import numpy as np
import cv2

def decode_zyb(zyb_path, width=250):
    valid_data = []

    with open(zyb_path, "r") as f:
        for line_number, line in enumerate(f, start=1):
            line_str = line.strip()

            try:
                val = int(line_str)
            except ValueError:
                print(f"Warning: Invalid integer on line {line_number}. Truncating and stopping.")
                break

            if not 0 <= val <= 127:
                print(f"Warning: Value {val} out of [0..127] range on line {line_number}. Truncating and stopping.")
                break

            valid_data.append(val)

    num_pixels = len(valid_data)
    remainder = num_pixels % width
    if remainder != 0:
        print(f"Note: Truncating last {remainder} pixel(s) to make rows complete.")
        num_pixels -= remainder
        valid_data = valid_data[:num_pixels]

    if num_pixels == 0:
        raise ValueError("No valid pixel data found after reading/truncating.")

    height = num_pixels // width
    img = np.zeros((height, width, 3), dtype=np.uint8)

    for i, val in enumerate(valid_data):
        r_2bits = (val >> 5) & 0b11      # 2 bits for Red
        g_3bits = (val >> 2) & 0b111    # 3 bits for Green
        b_2bits = val & 0b11           # 2 bits for Blue

        r = r_2bits * 64    # 0..3 -> 0..255 in steps of 64
        g = g_3bits * 32    # 0..7 -> 0..255 in steps of 32
        b = b_2bits * 64    # 0..3 -> 0..255 in steps of 64

        row = i // width
        col = i % width
        img[row, col] = [b, g, r]

    return img

def decrypt():
    zyb_file = 'image.zyb'
    width = 250

    img = decode_zyb(zyb_file, width=width)
    out_png = "image.png"
    cv2.imwrite(out_png, img)
    print(f"Decoded {zyb_file} (width={width}, height={img.shape[0]}) and saved as {out_png}")

if __name__ == "__main__":
    decrypt()

