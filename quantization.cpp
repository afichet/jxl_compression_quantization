
#include "quantization.h"

#include <cmath>
#include <cstring>
#include <cstdint>


template<typename T>
static T quantize_dequantize(T src, size_t n_bits)
{
    const uint64_t v = (1 << n_bits) - 1;
    return std::round(src * (T)v) / (T)v;
}


void quantize_dequantize_single_image(
    const std::vector<float>& input_image,
    std::vector<float>& output_image,
    size_t n_bits)
{
    output_image.resize(input_image.size());

    std::memcpy(output_image.data(), input_image.data(), sizeof(float) * input_image.size());

    #pragma omp parallel for
    for (size_t px = 0; px < input_image.size(); px++) {
        output_image[px] = quantize_dequantize(input_image[px], n_bits);
    }
}
