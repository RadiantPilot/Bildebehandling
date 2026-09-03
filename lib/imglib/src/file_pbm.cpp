/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#include <fstream>
#include <iostream>
#include <bitset>
#include "file_pbm.hpp"

namespace imglib::file::pbm {
header::header(std::uint32_t w, std::uint32_t h) 
{
    WIDTH = w;
    HEIGHT = h;
}
void header::parse_strings(std::string w, std::string h)
{
    WIDTH = std::stoi(w);
    HEIGHT = std::stoi(h);
}
std::vector<std::string> header::get_strings() const
{
    return {std::to_string(WIDTH),std::to_string(HEIGHT)};
}
bool read_file_type(std::ifstream& file) 
{
    std::string file_type;
    file >> file_type;
    return file_type != "P4";
}
header read_header(std::ifstream& file)
{
    if(read_file_type(file)) {
        std::cout << "Not a binary PBM file!" << '\n';
        return {};
    }
    std::string w,h;
    file >> w >> h;
    return {w,h};
}
bool read_data(std::ifstream& file, header& head, std::vector<double>& pixel_array)
{
    const std::uint8_t bits = 8;
    std::uint8_t in_data;
    file.read((char *)&in_data,1);
    std::uint32_t pad_width = head.width() % bits;
    pixel_array.resize(head.height()*head.width());
    for(std::uint32_t r = 0; r < head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width()-pad_width; c += bits) {
            file.read(reinterpret_cast<char*>(&in_data),sizeof(std::uint8_t));
            std::bitset<bits> in_byte(in_data);
            for(std::uint8_t k = 0; k < bits; ++k) {
                pixel_array[r * head.width() + c + k] = in_byte[bits-1-k] ? 0.0 : 1.0;
            }
        }
        if(pad_width > 0) {
            file.read(reinterpret_cast<char*>(&in_data),sizeof(std::uint8_t));
            std::bitset<bits> in_byte(in_data);
            for(std::uint8_t k = 0; k < pad_width;++k) {
                pixel_array[r * head.width() + head.width()-pad_width + k] = in_byte[bits-1-k] ? 0.0 : 1.0;
            }
        }
    }
    return true;
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
    if(head.width() == 0 || head.height() == 0) {
        std::cout << "Unable to read BMP file!\n";
    } else {
        read_data(infile, head, pixel_array);
    }
    infile.close();
    return head;
}
void write_file_type(std::ofstream& file) 
{
    std::string s = "P4 ";
    file.write(s.data(),s.size());
}
void write_header(std::ofstream& file, const header &head)
{
    auto strings = head.get_strings();
    std::string out_data = "";
    for(auto s : strings) {
        out_data += s + " ";
    }
    file.write(out_data.data(),out_data.size());
}
void write_data(std::ofstream& file, const header& head, const std::vector<double>& pixel_array)
{
    const std::uint8_t bits = 8;
    std::uint8_t pad_width = head.width() % bits;
    for(std::uint32_t r = 0; r < head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width()-pad_width; c+=bits) {
            std::bitset<bits> out_byte;
            for(std::uint8_t i = 0; i < bits; ++i){
                out_byte[bits-1-i] = pixel_array[r * head.width() + c + i] < 0.5;
            }
            file.write(reinterpret_cast<char*>(&out_byte),sizeof(std::uint8_t));
        }
        if(pad_width > 0) {
            std::bitset<bits> pad_byte;
            for(std::uint32_t i = 0; i < pad_width; ++i) {
                pad_byte[bits-1-i] = pixel_array[r * head.width() + head.width()-pad_width + i] < 0.5;
            }
            file.write(reinterpret_cast<char*>(&pad_byte),sizeof(std::uint8_t));
        }
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
    header head{cols,rows};
    write(file_name, head, pixel_array);
}
} // namespace imglib::pbm
