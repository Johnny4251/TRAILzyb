import cv2
import numpy as np

COLOR_COUNT = 128
PALETTE_COLS = 16
PALETTE_ROWS = COLOR_COUNT // PALETTE_COLS
IMAGE_SIZE = 500
CELL_WIDTH = IMAGE_SIZE // PALETTE_COLS
CELL_HEIGHT = IMAGE_SIZE // PALETTE_ROWS

palette_img = np.zeros((PALETTE_ROWS * CELL_HEIGHT, PALETTE_COLS * CELL_WIDTH, 3), dtype=np.uint8)
for i in range(COLOR_COUNT):
    r_2bits = (i >> 5) & 0b11
    g_3bits = (i >> 2) & 0b111
    b_2bits = i & 0b11

    r = r_2bits * 64
    g = g_3bits * 32
    b = b_2bits * 64

    row = i // PALETTE_COLS
    col = i % PALETTE_COLS
    top_left = (col * CELL_WIDTH, row * CELL_HEIGHT)
    bottom_right = ((col + 1) * CELL_WIDTH, (row + 1) * CELL_HEIGHT)

    cv2.rectangle(palette_img, top_left, bottom_right, (b, g, r), thickness=-1)

    text = str(i)
    font_scale = 0.4
    thickness = 1
    font = cv2.FONT_HERSHEY_SIMPLEX
    text_size, _ = cv2.getTextSize(text, font, font_scale, thickness)
    text_x = top_left[0] + (CELL_WIDTH - text_size[0]) // 2
    text_y = top_left[1] + (CELL_HEIGHT + text_size[1]) // 2

    text_color = (255, 255, 255) if (r + g + b) < 300 else (0, 0, 0)  # Contrast logic
    cv2.putText(palette_img, text, (text_x, text_y), font, font_scale, text_color, thickness, lineType=cv2.LINE_AA)

palette_img = cv2.resize(palette_img, (IMAGE_SIZE, IMAGE_SIZE), interpolation=cv2.INTER_NEAREST)

cv2.imwrite("palette.png", palette_img)
