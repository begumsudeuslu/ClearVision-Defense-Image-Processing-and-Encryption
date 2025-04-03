#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.

    width = image.get_width();
    height = image.get_height();

    int upperSize = width * (width+1) / 2;   //upper triangular matrix size
    int lowerSize =  width * (width-1) / 2;    //lower triangular matrix size

    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.

    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.

    int upperIndex = 0;
    int lowerIndex = 0;

    for (int i = 0; i<height; ++i)   {
        for (int j = 0; j< width; ++j)   {
            if (j >= i)   {    // the diognal is stored only once in the upper triangular matrix.
                upper_triangular[upperIndex++] = image.get_pixel(i,j);
            }  else  {      // for lower triangular matrix
                lower_triangular[lowerIndex++] = image.get_pixel(i, j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.

    width = w;
    height = h;

    // Since file reading part should dynamically allocate upper and lower matrices.

    upper_triangular = new int[width * (width +1) / 2];
    lower_triangular = new int[width * (width -1) / 2];
    
    
    // You should simply copy the parameters to instance variables.

    std::copy(upper, upper + width * (width + 1) / 2, upper_triangular);
    std::copy(lower, lower + width * (width - 1) / 2, lower_triangular);

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    
    delete[] upper_triangular;
    delete[] lower_triangular;

}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int upperIndex = 0;
    int lowerIndex = 0;

    // TODO: Your code goes here.
    for (int i = 0; i<height; ++i)    {
        for (int j = 0; j <width; ++j)   {
            if (j >= i)      {       // for upper triangular matrix
                image.set_pixel(i, j, upper_triangular[upperIndex++]);
            } else { // for lower triangular matrix
                image.set_pixel(i, j, lower_triangular[lowerIndex++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    
    if (image.get_width() != width || image.get_height() != height) {
        throw std::invalid_argument("Image dimensions do not match!");
    }

    int upperIndex = 0;
    int lowerIndex = 0;

    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.

    for (int i=0; i< height; ++i)   {
        for (int j=0; j<width; ++j)    {
            if (j >= i)    {           // for upper triangular matrix
                upper_triangular[upperIndex] = image.get_pixel(i, j);
                upperIndex++;
            } else     {               // for lower triangular matrix
                lower_triangular[lowerIndex] = image.get_pixel(i, j);
                lowerIndex++;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary); // binary mode to prevent extra new lines
    if (!file.is_open()) {
        throw std::runtime_error("File couldn't be opened.");
    }

    // 1. Write width and height on the first line, separated by a single space.
    file << width << " " << height << "\n";  // Use single newline (Unix style)

    // 2. Write the upper_triangular array to the second line.
    int upper_size = width * (width + 1) / 2;
    for (int i = 0; i < upper_size; ++i) {
        file << upper_triangular[i];
        if (i != upper_size - 1) {
            file << " ";  // Add a space between elements, but not after the last element
        }
    }
    file << "\n";  // End of second line with single newline

    // 3. Write the lower_triangular array to the third line.
    int lower_size = width * (width - 1) / 2;
    for (int i = 0; i < lower_size; ++i) {
        file << lower_triangular[i];
        if (i != lower_size - 1) {
            file << " ";  // Add a space between elements, but not after the last element
        }
    }
    file << "\n";  // End of third line with single newline

    file.close();
}


// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.

    std::ifstream file(filename);
    if(!file.is_open())  {
        throw std::runtime_error("File couldt be opened");
    }


    // 1. Open the file and read width and height from the first line, separated by a space.
    
    int width, height;
    file >> width >> height;
   
    // 2. Calculate the sizes of the upper and lower triangular arrays.

    int upper_size = width * (width+1) / 2;
    int lower_size = width * (width-1) / 2;

    // 3. Allocate memory for both arrays.

    int *upper_triangular = new int[upper_size];
    int *lower_triangular = new int[lower_size];

    // 4. Read the upper_triangular array from the second line, space-separated.
    
    for (int i = 0; i <upper_size; ++i)   {
        file >> upper_triangular[i];
    }
    
    // 5. Read the lower_triangular array from the third line, space-separated.

    for (int i = 0; i <lower_size; ++i)   {
        file >> lower_triangular[i];
    }

    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.

    file.close();

    return SecretImage(width, height, upper_triangular, lower_triangular);
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
