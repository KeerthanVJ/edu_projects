// Image.cpp
//Keerthan Vijayavel

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

// Default constructor
Image::Image() : Matrix(), filePath(""), numChannels(0), width(0), height(0) {}

Image::Image(const std::string& filePath) {
    // Load the image using stb_image
    int width, height, numChannels;
    uint8_t* imageData = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);

    if (!imageData) {
        throw std::runtime_error("Failure to load image at : " + filePath);
    }
        // Initialize the Matrix base class with the image data
    // (You need to implement a constructor or a function in the Matrix class
    //  that allows you to initialize it with image data)
    // ...

    
    Matrix::operator=(Matrix(height, width * numChannels));

    // Set the Image properties
    this->filePath = filePath;
    this->numChannels = numChannels;
    this->width = width;
    this->height = height;

    // Copy stb_image data into Matrix
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width; b++) {
            for (int c = 0; c < numChannels; c++) {
                data[a][b * numChannels + c] = imageData[a * width * numChannels + b * numChannels + c];
            }
        }
    }

    // Free the loaded image data
    stbi_image_free(imageData);
}

// Constructor with file path, channels, width, and height
Image::Image(const std::string& filePath, int numChannels, int width, int height) : Matrix(height, width * numChannels), filePath(filePath), numChannels(numChannels), width(width), height(height) {}

// Assignment operator
Image& Image::operator=(const Image& other) {
    if (this == &other) return *this;
    
    Matrix::operator=(other);
    filePath = other.filePath;
    numChannels = other.numChannels;
    width = other.width;
    height = other.height;
    
    return *this;
}

// Copy constructor
Image::Image(const Image& other) : Matrix(other), filePath(other.filePath), numChannels(other.numChannels), width(other.width), height(other.height) {}

// Destructor
Image::~Image() {
    //Destructor for Matrix should handle this
}

// Scaling an image
Image Image::operator*(double scalar) const {
    
    Image result(filePath, numChannels, width, height);
    
    
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width * numChannels; b++) {
            int val = data[a][b] * scalar;
            val = val < 0 ? 0 : (val > 255 ? 255 : val);
            result[a][b] = val;
        }
    }
    
    return result;
}

// Adding two images
Image Image::operator+(const Image& ope) const {
    if (width != ope.width || height != ope.height || numChannels != ope.numChannels) {
        throw std::invalid_argument("Dimensions do not match for +");
    }
    
    
    Image result(filePath, numChannels, width, height);
    
    
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width * numChannels; b++) {
            int v = data[a][b] + ope[a][b];
            result[a][b] = v > 255 ? 255 : v;
        }
    }
    
    return result;
}

// Subtracting two images
Image Image::operator-(const Image& ope) const {
    if (width != ope.width || height != ope.height || numChannels != ope.numChannels) {
        throw std::invalid_argument("Dimensions do not match for -");
    }
    
    
    Image result(filePath, numChannels, width, height);
    
    
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width * numChannels; b++) {
            int v = data[a][b] - ope[a][b];
            result[a][b] = v < 0 ? 0 : v;
        }
    }
    
    return result;
}

// Multiplying images
Image Image::operator*(const Image& ope) const {
    if (width != ope.width || height != ope.height || numChannels != ope.numChannels) {
        throw std::invalid_argument("Image size mismatch for *");
    }
    
    
    Image result(filePath, numChannels, width, height);
    
    
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width * numChannels; b++) {
            result[a][b] = (data[a][b] * ope[a][b]) / 255;
        }
    }
    
    return result;
}


int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

void Image::save(const std::string& pth) const {
    // Convert the Matrix data into a 1D array suitable for saving as an image
    std::vector<uint8_t> buf(width * height * numChannels);
    
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width; b++) {
            for (int c = 0; c < numChannels; c++) {
                buf[(a * width + b) * numChannels + c] = data[a][b * numChannels + c];
            }
        }
    }
    
    // Save the image data to the specified file using stb_image_write
    stbi_write_png(pth.c_str(), width, height, numChannels, buf.data(), width * numChannels);
}

void Image::resize(int nWidth, int nHeight) {
    // YOUR CODE HERE
    std::vector<uint8_t> other(width * height * numChannels);
    std::vector<uint8_t> dst(nWidth * nHeight * numChannels);
    
   
    for (int a = 0; a < height; a++) {
        for (int b = 0; b < width; b++) {
            for (int c = 0; c < numChannels; c++) {
                other[(a * width + b) * numChannels + c] = data[a][b * numChannels + c];
            }
        }
    }
    
   //resizing
    stbir_resize_uint8(other.data(), width, height, width * numChannels,
                      dst.data(), nWidth, nHeight, nWidth * numChannels, numChannels);
    
   
    *this = Image(filePath, numChannels, nWidth, nHeight);
    
    
    for (int a = 0; a < nHeight; a++) {
        for (int b = 0; b < nWidth; b++) {
            for (int c = 0; c < numChannels; c++) {
                data[a][b * numChannels + c] = dst[(a * nWidth + b) * numChannels + c];
            }
        }
    }
    
    // Dimension assignment
    width = nWidth;
    height = nHeight;
}
