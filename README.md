# ClearVision-Defense-Image-Processing-and-Encryption
ClearVision Defense is an advanced application designed to enhance and safeguard critical imagery used in defense operations. The project focuses on implementing robust image filters, improving image clarity, and securing sensitive information through encryption and decryption within images. This system utilizes grayscale image processing, as well as secure message embedding, extraction, and image reconstruction using the concepts of upper and lower triangular matrices.

The core functionality is implemented in C++ using the stb_image.h library for image reading and writing, and custom encryption techniques based on the Least Significant Bit (LSB) method for message embedding.

Key Features
  - Image Filtering: Image processing includes operations such as mean filtering, Gaussian smoothing, and unsharp masking to reduce noise and improve image clarity.
  - Image Encryption and Decryption: The system allows secure embedding and extraction of sensitive messages using encryption techniques based on LSB encoding.
  - Secure Image Storage: The SecretImage class stores images in a secure manner using upper and lower triangular matrices, which are then reconstructed into a full grayscale image.
  - Dynamic Memory Allocation: The project leverages dynamic memory allocation for efficient handling of large image data.

Classes and Functionality
GrayscaleImage Class
This class is responsible for handling grayscale images, providing basic image manipulation operations, and supporting the functionality needed for image processing and filtering. The key features include:
  - Constructors: Multiple constructors for loading images from a file, initializing from pre-existing data, and creating blank images.
  - Operators: Overloaded operators for addition, subtraction, and equality checking between grayscale images.
  - Image Saving: Functionality to save an image in PNG format.
  - Pixel Access: Functions to access and modify individual pixel values.
    
Important Functions
GrayscaleImage(const char* filename): Constructor that loads an image from a file.
GrayscaleImage(int** inputData, int h, int w): Initializes the image from a 2D array of pixel data.
GrayscaleImage(int w, int h): Creates a blank image of the specified size.
operator+, operator-, operator==: Performs addition, subtraction, and comparison on images.
save_to_file(const char* filename): Saves the image to a file.

SecretImage Class
This class securely stores images by splitting them into upper and lower triangular matrices. It supports image encryption and decryption, and ensures the integrity and confidentiality of the image data.

Key Features
  - Image Storage: The image is split into upper and lower triangular matrices for secure storage.
  - Reconstruction: The image can be reconstructed by combining the upper and lower triangular matrices.
  - Save and Load: Functions to save and load the image data to/from a file.

Important Functions
SecretImage(const GrayscaleImage& image): Splits a GrayscaleImage into its upper and lower triangular matrices.
reconstruct(): Reconstructs the full image from the triangular matrices.
save_back(const GrayscaleImage& image): Updates the triangular matrices with new image data.
save_to_file(const std::string& filename): Saves the upper and lower triangular matrices to a file.
load_from_file(const std::string& filename): Loads a SecretImage from a file.

Crypto Class
The Crypto class is responsible for embedding secret messages into the least significant bits (LSB) of image pixels. It also supports extracting embedded messages and provides encryption/decryption functionality using LSB encoding.

Key Features
  - Message Embedding: The embed_lsbs function embeds a secret message into the image using the least significant bits of the pixel values.
  - Message Extraction: The extract_lsbs function retrieves the embedded message by decoding the least significant bits from the image.
  - Encryption and Decryption: Functions to encrypt and decrypt messages using LSB encoding, ensuring that sensitive information can be securely embedded and later retrieved.

Important Functions
embed_lsbs(const std::string& message, GrayscaleImage& image): Embeds a message into the image by modifying the least significant bits of the image pixels.
extract_lsbs(const GrayscaleImage& image, size_t message_length): Extracts an embedded message from the image.
encrypt_message(const std::string& message, GrayscaleImage& image): Encrypts a message and embeds it within the image.
decrypt_message(const GrayscaleImage& image): Extracts and decrypts the hidden message from the image.


