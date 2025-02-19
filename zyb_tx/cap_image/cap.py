import cap_image
import decrypt
import encrypt
import rescaler


def main():
    cap_image.capture_image()
    encrypt.encrypt()
    decrypt.decrypt()
    rescaler.rescale()

if __name__ == "__main__":
    main()
