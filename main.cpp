#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <cstring>

// #define USE_PNG

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

    for (size_t q = 8; q > 1; q--) {
        std::stringstream ss_lossless;
        ss_lossless << "lossless_quantization_jxl_" << q << ".jxl";

        compress_framebuffer(
            framebuffer,
            ss_lossless.str().c_str(),
            width, height,
            q, 0, // Quantization
            0,    // Compression
            1     // Downsampling
        );

        std::stringstream ss_lossy;
        ss_lossy << "lossy_quantization_jxl_" << q << ".jxl";

        compress_framebuffer(
            framebuffer,
            ss_lossy.str().c_str(),
            width, height,
            q, 0,    // Quantization
            1,       // Compression
            1        // Downsampling
        );
    }

    return 0;
}
