#pragma once

#include <vector>
#include <cstddef>


void quantize_dequantize_single_image(
    const std::vector<float>& input_image,
    std::vector<float>& output_image,
    size_t n_bits);
