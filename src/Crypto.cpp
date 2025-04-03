#include "Crypto.h"
#include "GrayscaleImage.h"

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // Step 1: Reconstruct the SecretImage to a GrayscaleImage
    GrayscaleImage grayscale_image = secret_image.reconstruct(); // Assuming you have a reconstruct method

    // Step 2: Calculate the image dimensions
    int width = grayscale_image.get_width(); 
    int height = grayscale_image.get_height(); 

    // Step 3: Determine the total bits required based on message length
    int total_bits_needed = message_length * 7; // Each character is represented by 7 bits

    // Step 4: Ensure the image has enough pixels; if not, throw an error
    if (width * height < total_bits_needed) {
        throw std::invalid_argument("Not enough pixels to extract the LSBs for the given message length.");
    }

    // Step 5: Calculate the starting pixel based on the message length
    int starting_pixel_index = (width * height) - total_bits_needed; // Start from the last pixel

    // Step 6: Extract LSBs from the image pixels
    for (int i = starting_pixel_index; i < width * height; ++i) {
        int row = i / width;     // Calculate row from the linear index
        int col = i % width;     // Calculate column from the linear index
        int pixel_value = grayscale_image.get_pixel(row, col);
        LSB_array.push_back(pixel_value & 1); 
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size()%7 != 0)   {
        throw std::invalid_argument("LSB array size must be a multiple of 7");
    } 
    
    
    // 2. Convert each group of 7 bits into an ASCII character.
    for (size_t i = 0; i<LSB_array.size(); i+=7)   {
        int ascii_value = 0;
        for (int j = 0; j<7; ++j)   {
            ascii_value =(ascii_value<<1)|LSB_array[i+j];
        }

        message += static_cast<char>(ascii_value);
    }
    
    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.

    for (char c : message) {
    for (int j = 6; j >= 0; --j) { // Start from the most significant bit
        LSB_array.push_back((c >> j) & 1);
    }
}

    // 3. Return the array of bits.
    return LSB_array;
}

SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // Create a SecretImage object to store the final result
    //SecretImage secret_image(nullptr);
    //secret_image = image;
    SecretImage secret_image(image); //if i do like that, i wouldnt get any embed runtime error

    int get_width = image.get_width();
    int get_height = image.get_height();

    // Step 1: Check if the image can store the LSB array
    int total_pixels = get_width * get_height;
    int messageLen = LSB_array.size();
    int messageBit = messageLen*7;

    if (messageBit > total_pixels) {
        throw std::runtime_error("Message is too large to embed in the image.");
    }

    // Step 2: Calculate the starting pixel
    int start_pixel = total_pixels - messageBit; // The pixel where embedding starts

    // Step 3: Embed the bits into the least significant bits of the image pixels
    int bit_index = 0; // Track the position in the LSB_array
    for (int pixel_index = start_pixel; pixel_index < total_pixels && bit_index<messageBit; ++pixel_index) {
        int row = pixel_index / get_width; // Calculate row
        int col = pixel_index % get_width; // Calculate column

        int pixel_value = image.get_pixel(row, col); // Get the current pixel value

        if (pixel_value==255)   {
            bit_index++;
            continue;

        }  else   {
            pixel_value = (pixel_value & ~1) | LSB_array[bit_index];
        }

        // Set the modified pixel value back into the image
        image.set_pixel(row, col, pixel_value);

        // Move to the next bit in the message
        bit_index++;
    }

    // Step 4: Save the modified image back into the secret image using triangular matrices
    secret_image.save_back(image); // Call to save_back to store the modified image data

    // Return the SecretImage object with the embedded message
    return secret_image;
}
