#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
    }

    // TODO: Your code goes here.
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    
    data = new int*[height];
    for (int i = 0; i< height; i++)  {
        data[i] = new int[width];
        for (int j = 0; j<width; ++j)   {
            data[i][j] = image[i*width+j];   // Fill the matrix with pixel values from the image
        }
    }


    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    // TODO: Your code goes here.
    height = h;
    width = w;

    // Don't forget to dynamically allocate memory for the matrix.
    data = new int*[height];
    for (int i = 0; i < height; ++i)  {
        data[i] = new int[width];
        std::memcpy(data[i], inputData[i], width * sizeof(int)); // Copy the values
    }
    // Initialize the image with a pre-existing data matrix by copying the values.
  
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // TODO: Your code goes here.
    width = w;
    height = h;

    data = new int*[height];
    for(int i = 0; i<height; ++i)   {
        data[i] = new int[width]();   // initialize
    }

    // Just dynamically allocate the memory for the new matrix.

}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    // TODO: Your code goes here.
    width = other.width;
    height = other.height;

    // Copy constructor: dynamically allocate memory and 
    data = new int*[height];
    for (int i = 0; i<height; ++i)  {
        data[i] = new int[width];
        std::memcpy(data[i], other.data[i], width * sizeof(int)); // Copy pixel values
    }

}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.

    for (int i = 0; i<height; ++i)  {
        delete[] data[i];
    }
    delete[] data;
    
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.

    if (width != other.width ||  height != other.height)     {    // different dimensions
        return false;
    }

    for (int i = 0; i<height; ++i)    {
        for(int j = 0; j<width; ++j)    {
            if (data[i][j] != other.data[i][j])   {
                return false;     // found different pixel value
            }
        }
    }
    
    // If they do, return true.
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    
    // check the dimensions of the images are same or not
    if (width != other.width || height!= other.height)  {
        throw std::invalid_argument("Images must have the same dimensions.");
    }
    
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.

    for (int i = 0; i<height; ++i)   {
        for (int j = 0; j<width; ++j)  {
            int sum = data[i][j] + other.data[i][j];
            //clamping
            if (sum > 255)  {
                result.set_pixel(i, j, 255);      // result.data[][] etc. another way
            } else  {                           // sum cannot be less than 0
                result.set_pixel(i, j, sum);
            }
        }
    }

    return result;   // return new image
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    
    // check the dimensions of the images is same or not
    if (width != other.width || height != other.height)   {
        throw std::invalid_argument("Images must have the same dimesnions.");
    }
    
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.

    for (int i = 0; i<height; ++i)  {
        for  (int j=0; j<width; ++j)   {
            int difference = data[i][j] - other.data[i][j];
            // clamping
            if(difference<0)   {                    
                result.set_pixel(i, j, 0);
            } else {                            // difference cannot be bigger than 255
                result.set_pixel(i, j, difference);
            }
        }
    }

    return result;    // return new image
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
