#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <cstring>

#define USE_PNG

#ifdef USE_PNG
#include "lodepng.h"
#endif // USE_PNG

#include "jxl_helpers.h"
#include "quantization.h"


int main(int argc, char* argv[])
{
    std::vector<float> framebuffer;
    unsigned int width, height;

#ifdef USE_PNG
    if (argc < 2) {
        std::cout << "Usage:" << std::endl
                  << "------" << std::endl
                  << argv[0] << " <png file>" << std::endl;
        return 0;
    }

    std::vector<uint8_t> png_colors;
    unsigned int w_png, h_png;

    lodepng::decode(png_colors, width, height, argv[1]);

    framebuffer.resize(width * height);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            framebuffer[y * width + x] = (float)(
                png_colors[4 * (y * width + x) + 0] +
                png_colors[4 * (y * width + x) + 1] +
                png_colors[4 * (y * width + x) + 2]
            ) / (float)(3 * 255);
        }
    }
#else
    (void)argc; (void)argv;

    width = 640;
    height = 480;

    framebuffer.resize(width * height);

    // Generate a gradient to check if the quantization works as expected
    for (unsigned int y = 0; y < height; y++) {
        const float value = (float)y / (float)(height - 1);

        for (unsigned int x = 0; x < width; x++) {
            framebuffer[y * width + x] = value;
        }
    }
#endif // USE_PNG

    // Save the framebuffer with different a different quantization

    // Lossless - let JXL figures out the quantization
    for (size_t q = 8; q > 1; q--) {
        std::stringstream ss;
        ss << "lossless_quantization_jxl_" << q << "_float.jxl";

        compress_framebuffer_float(
            framebuffer,
            ss.str().c_str(),
            width, height,
            q, 0, // Quantization
            0,    // Compression
            1     // Downsampling
        );
    }

    for (size_t q = 8; q > 1; q--) {
        std::stringstream ss;
        ss << "lossless_quantization_jxl_" << q << "_uint8.jxl";

        compress_framebuffer_uint8(
            framebuffer,
            ss.str().c_str(),
            width, height,
            q, 0, // Quantization
            0,    // Compression
            1     // Downsampling
        );
    }

    // float quality = 1.f;

    for (float quality = 0.1f; quality < 8.f; quality += 0.5f) {
        // Lossy - let JXL figures out the quantization
        for (size_t q = 8; q > 1; q--) {
            std::stringstream ss;
            ss << "lossy_quantization_jxl_" << q << "_float_" << quality << ".jxl";

            compress_framebuffer_float(
                framebuffer,
                ss.str().c_str(),
                width, height,
                q, 0,    // Quantization
                quality, // Compression
                1        // Downsampling
            );
        }

        for (size_t q = 8; q > 1; q--) {
            std::stringstream ss;
            ss << "lossy_quantization_jxl_" << q << "_uint8_" << quality << ".jxl";

            compress_framebuffer_uint8(
                framebuffer,
                ss.str().c_str(),
                width, height,
                q, 0,    // Quantization
                quality, // Compression
                1        // Downsampling
            );
        }

        // Lossy - Enforce a quantization step before
        for (size_t q = 8; q > 1; q--) {
            std::stringstream ss;
            ss << "sw_lossy_quantization_jxl_" << q << "_float_" << quality << ".jxl";

            std::vector<float> quantized_framebuffer;

            quantize_dequantize_single_image(
                framebuffer,
                quantized_framebuffer,
                q);

            compress_framebuffer_float(
                quantized_framebuffer,
                ss.str().c_str(),
                width, height,
                q, 0,    // Quantization
                quality, // Compression
                1        // Downsampling
            );
        }

        for (size_t q = 8; q > 1; q--) {
            std::stringstream ss;
            ss << "sw_lossy_quantization_jxl_" << q << "_uint8_" << quality << ".jxl";

            std::vector<float> quantized_framebuffer;

            quantize_dequantize_single_image(
                framebuffer,
                quantized_framebuffer,
                q);

            compress_framebuffer_uint8(
                quantized_framebuffer,
                ss.str().c_str(),
                width, height,
                q, 0,    // Quantization
                quality, // Compression
                1        // Downsampling
            );
        }
    }

    return 0;
}
