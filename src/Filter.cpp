#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.

    int width = image.get_width();
    int height = image.get_height();
    int halfKernelSize = kernelSize/2;

    // 1. Copy the original image for reference.

    GrayscaleImage originalImage = image;

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    for (int i = 0; i < height; ++i)    {
        for (int j = 0; j<width; ++j)     {
            int sum = 0;
            int count = 0;

            for (int ki = -halfKernelSize; ki <= halfKernelSize; ++ki)   {
                for (int kj = -halfKernelSize; kj <= halfKernelSize; ++kj)   {
                    int ni = i + ki;     // neighbor row index
                    int nj = j + kj;     // neighbor column index

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)  {     // check the bounds of pixel index and pixels neighbor index
                        sum += originalImage.get_pixel(ni, nj);
                    }
                    count ++;
                }
            }

            // 3. Update each pixel with the computed mean.
            int avg = sum / count;
            image.set_pixel(i, j, avg);
        }
    }   

  
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.  

    int width = image.get_width();
    int height = image.get_height();
    int halfKernelSize = kernelSize / 2;

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0;

    // 1. Create a Gaussian kernel based on the given sigma value.
    for (int i = - halfKernelSize; i <= halfKernelSize; ++i)     {
        for (int j = -halfKernelSize; j <= halfKernelSize; ++j) {
            double exponent = - (i * i + j * j) / (2.0 * sigma * sigma);
            kernel[i + halfKernelSize][j + halfKernelSize] = (1.0 / (2.0 * M_PI * sigma * sigma)) * std::exp(exponent);
            sum += kernel[i + halfKernelSize][j + halfKernelSize];
        }
    }
    
    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    GrayscaleImage originalImage = image;

    // 3. For each pixel, compute the weighted sum using the kernel.

    for (int i = 0; i<height; ++i)  {
        for (int j = 0; j<width; ++j)  {
            double pixel_val = 0;

            for (int ki = -halfKernelSize; ki <= halfKernelSize; ++ki)  {
                for (int kj = -halfKernelSize; kj <= halfKernelSize; ++kj)   {
                    int ni = i + ki;
                    int nj = j + kj;

                    if(ni>= 0 && nj >=0 && ni<height && nj<width)     {
                        double get_pixel = originalImage.get_pixel(ni, nj);
                        pixel_val += get_pixel*kernel[ki+halfKernelSize][kj+halfKernelSize];

                    }
                }
            }
        // 4. Update the pixel values with the smoothed results.
        pixel_val = std::max(0.0, std::min(255.0, pixel_val)); 
        image.set_pixel(i,j,static_cast<int>(std::floor(pixel_val)));
        }
    }

   
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.

    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage originalImage = image;
    GrayscaleImage blurredImage = image;
    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    
    int width = image.get_width();
    int height = image.get_height();
    //GrayscaleImage edgeImage(width, height);

    for (int i = 0; i<height; ++i)     {
        for (int j = 0; j<width; ++j)     {
            int originalPixel = originalImage.get_pixel(i, j);
            int blurredPixel = blurredImage.get_pixel(i, j);
            double edgePixel  = (originalPixel - blurredPixel)*amount;

            double sharpenedPixel = originalPixel + edgePixel;

            // 3. Clip values to ensure they are within a valid range [0-255].
            sharpenedPixel = std::max(0, std::min(255, static_cast<int>(sharpenedPixel)));

            image.set_pixel(i, j, sharpenedPixel);

        }
        
    }
   
}
