/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#include <fstream>
#include <iostream>
#include <algorithm>
#include "file_pgm.hpp"
namespace imglib::file::pgm {
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
    return {std::to_string(WIDTH),std::to_string(HEIGHT),std::to_string(255)};
}
bool read_file_type(std::ifstream& file) 
{

    std::string file_type;
    file >> file_type;
    return file_type != "P5";
}
header read_header(std::ifstream& file)
{
    if(read_file_type(file)) {
        std::cout << "Not a PGM file!" << '\n';
        return {};
    }
    std::string w,h,max;
    file >> w >> h >> max;
    return {w,h};
}
void read_data(std::ifstream& file, header& head, std::vector<double>& pixel_array)
{
    std::uint8_t in_data;
    file.read(reinterpret_cast<char*>(&in_data),sizeof(std::uint8_t));
    pixel_array.resize(head.width() * head.height());
    for(std::uint32_t r = 0; r < head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width(); ++c) {
            file.read(reinterpret_cast<char*>(&in_data), sizeof(std::uint8_t));
            pixel_array[head.width() * r + c] = static_cast<double>(in_data);
        }
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
    if(head.width() == 0 || head.height() == 0) {
        std::cout << "Unable to read PGM file!\n";
    } else {
        read_data(infile, head, pixel_array);
    }
    infile.close();
    return head;
}
void write_file_type(std::ofstream& file) 
{
    std::string s = "P5 ";
    file.write(s.data(), s.size());
}
void write_header(std::ofstream& file, const header &head)
{
    auto strings = head.get_strings();
    std::string out_data = "";
    for(auto s : strings) {
        out_data +=  s + " ";
    }
    file.write(out_data.data(),out_data.size());
}
void write_data(std::ofstream& file, const header& head, const std::vector<double>& pixel_array)
{
    std::uint8_t out_data;
    for(std::uint32_t r = 0; r < head.height(); ++r) {
        for(std::uint32_t c = 0; c < head.width(); ++c) {
            out_data = static_cast<uint8_t>(std::clamp(pixel_array[r*head.width()+c], 0.0, 255.0));
            file.write(reinterpret_cast<const char*>(&out_data), sizeof(std::uint8_t));
        }
    }
}
void write(std::string file_name, header& head, const std::vector<double> &pixel_array)
{
    std::ofstream outfile;
    outfile.open(file_name, std::ios::binary | std::ios::out);

    write_file_type(outfile);
    write_header(outfile, head);
    write_data(outfile, head, pixel_array);

    outfile.close();
}
void write(std::string file_name, std::uint32_t rows, std::uint32_t cols, const std::vector<double> &pixel_array)
{
    header head{cols,rows};
    write(file_name, head, pixel_array);
}
} // namespace imglib::pgm
