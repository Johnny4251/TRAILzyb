import cv2
import os

def rescale_image(input_filename, output_filename, size=(500, 500)):
    if not os.path.exists(input_filename):
        print(f"Error: {input_filename} not found.")
        exit()
    
    image = cv2.imread(input_filename)
    image_resized = cv2.resize(image, size, interpolation=cv2.INTER_AREA)
    cv2.imwrite(output_filename, image_resized)
    print(f"Rescaled image saved as {output_filename}")

def rescale():
    input_image = "image.png"
    output_image = "image_rescaled.png"
    
    rescale_image(input_image, output_image)


if __name__ == "__main__":
    rescale()
