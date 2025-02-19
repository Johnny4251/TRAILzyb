import cv2
import os

def rescale_image(input_filename, output_filename, size=(500, 500)):
    """Rescales an image to the specified size and saves it."""
    if not os.path.exists(input_filename):
        print(f"Error: {input_filename} not found.")
        exit()
    
    # Read the image
    image = cv2.imread(input_filename)
    
    # Resize the image to 500x500
    image_resized = cv2.resize(image, size, interpolation=cv2.INTER_AREA)
    
    # Save the resized image
    cv2.imwrite(output_filename, image_resized)
    print(f"Rescaled image saved as {output_filename}")

def rescale():
    # Define input and output filenames
    input_image = "image.png"
    output_image = "image_rescaled.png"
    

    # Perform rescaling
    rescale_image(input_image, output_image)


if __name__ == "__main__":
    rescale()
