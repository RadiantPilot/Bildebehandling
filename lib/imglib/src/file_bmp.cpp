/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#include <fstream>
#include <iostream>
#include <algorithm>
#include "file_bmp.hpp"
#include "utils.hpp"
namespace imglib::file::bmp {
enum class bmp_field : std::size_t
{
    //File header
    file_size = 0,
    reserved = 4,
    offset = 8,
    //DIB header (BITMAPINFOHEADER)
    header_size = 12,
    width = 16,
    height = 20,
    color_planes = 24,
    bits_per_pixel = 26,
    compression = 28,
    image_size = 32,
    horizontal_resolution = 36,
    vertical_resolution = 40,
    number_of_colors = 44,
    number_of_important_colors = 48
};

header::header(std::uint32_t rows, std::uint32_t cols) 
{
    RESERVED = 0;
    OFFSET = 54;
    HEADER_SIZE = 40;
    WIDTH = cols;
    HEIGHT = rows;
    COLOR_PLANES = 1;
    BITS_PER_PIXEL = 24;
    COMPRESSION = 0;
    IMAGE_SIZE = 0;
    HORIZONTAL_RESOLUTION = 0;
    VERTICAL_RESOLUTION = 0;
    NUMBER_OF_COLORS = 0;
    NUMBER_OF_IMPORTANT_COLORS = 0;

    FILE_SIZE = OFFSET + 3*HEIGHT*WIDTH + HEIGHT*(WIDTH%4);
}
void header::parse_bytes(std::array<std::uint8_t,52> bytes)
{
    concat_bytes(bytes.data(), bmp_field::file_size, FILE_SIZE);
    concat_bytes(bytes.data(), bmp_field::reserved, RESERVED);
    concat_bytes(bytes.data(), bmp_field::offset, OFFSET);
    concat_bytes(bytes.data(), bmp_field::header_size, HEADER_SIZE);
    concat_bytes(bytes.data(), bmp_field::width, WIDTH);
    concat_bytes(bytes.data(), bmp_field::height, HEIGHT);
    concat_bytes(bytes.data(), bmp_field::color_planes, COLOR_PLANES);
    concat_bytes(bytes.data(), bmp_field::bits_per_pixel, BITS_PER_PIXEL);
    concat_bytes(bytes.data(), bmp_field::compression, COMPRESSION);
    concat_bytes(bytes.data(), bmp_field::image_size, IMAGE_SIZE);
    concat_bytes(bytes.data(), bmp_field::horizontal_resolution, HORIZONTAL_RESOLUTION);
    concat_bytes(bytes.data(), bmp_field::vertical_resolution, VERTICAL_RESOLUTION);
    concat_bytes(bytes.data(), bmp_field::number_of_colors, NUMBER_OF_COLORS);
    concat_bytes(bytes.data(), bmp_field::number_of_important_colors, NUMBER_OF_IMPORTANT_COLORS);
}
std::array<std::uint8_t, 52> header::get_bytes() const
{
    std::array<std::uint8_t, 52> bytes;
    split_bytes(bytes.data(), bmp_field::file_size, FILE_SIZE);
    split_bytes(bytes.data(), bmp_field::reserved, RESERVED);
    split_bytes(bytes.data(), bmp_field::offset, OFFSET);
    split_bytes(bytes.data(), bmp_field::header_size, HEADER_SIZE);
    split_bytes(bytes.data(), bmp_field::width, WIDTH);
    split_bytes(bytes.data(), bmp_field::height, HEIGHT);
    split_bytes(bytes.data(), bmp_field::color_planes, COLOR_PLANES);
    split_bytes(bytes.data(), bmp_field::bits_per_pixel, BITS_PER_PIXEL);
    split_bytes(bytes.data(), bmp_field::compression, COMPRESSION);
    split_bytes(bytes.data(), bmp_field::image_size, IMAGE_SIZE);
    split_bytes(bytes.data(), bmp_field::horizontal_resolution, HORIZONTAL_RESOLUTION);
    split_bytes(bytes.data(), bmp_field::vertical_resolution, VERTICAL_RESOLUTION);
    split_bytes(bytes.data(), bmp_field::number_of_colors, NUMBER_OF_COLORS);
    split_bytes(bytes.data(), bmp_field::number_of_important_colors, NUMBER_OF_IMPORTANT_COLORS);
    return bytes;
}
bool read_file_type(std::ifstream& file) 
{
    std::string file_type(2,'\0');
    file.read(file_type.data(), file_type.size());
    return file_type != "BM";
}
header read_header(std::ifstream& file)
{
    if(read_file_type(file)) {
        std::cout << "Not a BMP file!" << '\n';
        return {};
    }
    std::array<std::uint8_t, 52> bytes;
    file.read(reinterpret_cast<char*>(bytes.data()), bytes.size());
    return {bytes};
}
void read_data(std::ifstream& file, header& head, std::vector<double>& pixel_array)
{
    std::uint8_t pixel[3];
    std::uint8_t padding = head.width() % 4;
    pixel_array.resize(head.width() * head.height());
    file.seekg(head.offset(), std::ios::beg);
    for(std::uint32_t r = 1; r <= head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width(); ++c) {
            file.read(reinterpret_cast<char*>(pixel), 3);
            pixel_array[head.width() * (head.height()-r) + c] = (pixel[0] + pixel[1] + pixel[2]) / 3.0;
        }
        file.seekg(padding,std::ios::cur);
    }
}
header read(std::string file_name, std::vector<double> &pixel_array)
{
    std::ifstream infile;
    infile.open(file_name, std::ios::binary | std::ios::in);
    if(infile.fail()) {
        std::cout << "Error opening file!" << "\n";
        return {};
    }
    auto head = read_header(infile);
    if(head.file_size() == 0) {
        std::cout << "Unable to read BMP file!\n";
    } else {
        read_data(infile, head, pixel_array);
    }
    infile.close();
    return head;
}
void write_file_type(std::ofstream& file) 
{
    file.write("BM",2);
}
void write_header(std::ofstream& file, const header &head)
{
    auto bytes = head.get_bytes();
    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}
void write_data(std::ofstream& file, const header& head, const std::vector<double>& pixel_array)
{
    std::string padding(head.width()%4,'\0');
    file.seekp(head.offset(), std::ios::beg);
    for(std::uint32_t r = 1; r <= head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width(); ++c) {
            std::uint8_t pixel_value = static_cast<uint8_t>(std::clamp(pixel_array[(head.height()-r) * head.width() + c], 0.0, 255.0));
            std::uint8_t out_data[] = {pixel_value,pixel_value,pixel_value};
            file.write(reinterpret_cast<const char*>(out_data), 3);
        }
        file.write(padding.data(),padding.size());
    }
}
void write(std::string file_name, const header& head, const std::vector<double> &pixel_array)
{
    std::ofstream outfile;
    outfile.open(file_name, std::ios::binary | std::ios::out);

    write_file_type(outfile);
    write_header(outfile, head);
    write_data(outfile, head, pixel_array);

    outfile.close();
}
void write(std::string file_name, std::uint32_t rows, uint32_t cols, const std::vector<double> &pixel_array)
{
    header head{rows,cols};
    write(file_name, head, pixel_array);
}
} // namespace imglib::bmp
