/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _BMP_HANDLER_
#define _BMP_HANDLER_
#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include "file.hpp"
namespace imglib::file::bmp {
class header : public file::header
{
private:
    std::uint32_t FILE_SIZE;
    std::uint32_t RESERVED;
    std::uint32_t OFFSET;
    std::uint32_t HEADER_SIZE;
    std::uint32_t WIDTH;
    std::uint32_t HEIGHT;
    std::uint16_t COLOR_PLANES;
    std::uint16_t BITS_PER_PIXEL;
    std::uint32_t COMPRESSION;
    std::uint32_t IMAGE_SIZE;
    std::uint32_t HORIZONTAL_RESOLUTION;
    std::uint32_t VERTICAL_RESOLUTION;
    std::uint32_t NUMBER_OF_COLORS;
    std::uint32_t NUMBER_OF_IMPORTANT_COLORS;
public:
    header() = default;
    header(std::uint32_t rows, std::uint32_t cols);
    header(std::array<uint8_t,52> bytes) {parse_bytes(bytes);}
    std::uint32_t file_size() const {return FILE_SIZE;}
    std::uint32_t offset() const {return OFFSET;}

    std::uint32_t width() const override {return WIDTH;}
    std::uint32_t height() const override {return HEIGHT;}

    void parse_bytes(std::array<uint8_t,52> bytes);
    std::array<uint8_t,52> get_bytes() const;
};

header read(std::string file_name, std::vector<double> &pixel_table);
void write(std::string file_name, const header& head, const std::vector<double> &pixel_table);
void write(std::string file_name, std::uint32_t rows, std::uint32_t cols, const std::vector<double> &pixel_table);
} // namespace imglib::bmp
#endif
