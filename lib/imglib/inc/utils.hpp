/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef UTILS_H
#define UTILS_H
#include "image.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace imglib {
template <typename T, typename E>
inline void concat_bytes(std::uint8_t bytes[], E offset, T& data)
{
    data = 0;
    auto N = static_cast<std::size_t>(offset);
    for(std::size_t i = 0; i < sizeof(T); ++i) {
        data |= bytes[N+i]<<i*8;
    }
}
template <typename T, typename E>
inline void split_bytes(std::uint8_t bytes[], E offset, T data)
{
    auto N = static_cast<std::size_t>(offset);
    for(std::size_t i = 0; i < sizeof(T); ++i) {
        bytes[N+i] = static_cast<uint8_t>(data>>i*8);
    }
}

std::string file_type(std::string file_name);
bool read_image(std::string file_name, std::uint32_t& h, std::uint32_t& w, std::vector<double>& pixel_array);
void save_image(std::string file_name, std::uint32_t h, std::uint32_t w, const std::vector<double>& pixel_array);

enum class padding_type
{
        zero,
        copy,
        mirror
};
image pad_image(image in,unsigned int h1, unsigned int h2, unsigned int w1, unsigned int w2, padding_type p);
}
#endif
