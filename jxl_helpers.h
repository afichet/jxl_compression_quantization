#pragma once

#include <vector>
#include <cstdint>


void compress_framebuffer_uint8(
    const std::vector<float>& framebuffer_in,
    const char* filename,
    uint32_t width, uint32_t height,
    uint32_t bits_per_sample,
    uint32_t exponent_bits_per_sample,
    float frame_distance,
    uint32_t downsampling_ratio);


void compress_framebuffer_float(
    const std::vector<float>& framebuffer_in,
    const char* filename,
    uint32_t width, uint32_t height,
    uint32_t bits_per_sample,
    uint32_t exponent_bits_per_sample,
    float frame_distance,
    uint32_t downsampling_ratio);
