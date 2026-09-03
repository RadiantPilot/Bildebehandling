/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#include <iostream>
#include "utils.hpp"
#include "file_types.hpp"
namespace imglib {
std::string file_type(std::string file_name)
{
    std::size_t f = file_name.find_last_of(".");
    if(f == std::string::npos) {
        return "";
    }
    return file_name.substr(f+1);
}
bool read_image(std::string file_name, std::uint32_t& h, std::uint32_t& w, std::vector<double>& pixel_array)
{
    auto type = file_type(file_name);
    if(type == "") {
        std::cout << "File name is missing file extension!\n";
        h = 0; w = 0;
    } else if(type == "bmp") {
        auto head = file::bmp::read(file_name, pixel_array);
        h = head.height(); w = head.width();
    } else if(type == "pgm") {
        auto head = file::pgm::read(file_name, pixel_array);
        h = head.height(); w = head.width();
    } else if(type == "pbm") {
        auto head = file::pbm::read(file_name, pixel_array);
        h = head.height(); w = head.width();
    } else {
        std::cout << "File format not supported!\n";
        h = 0; w = 0;
    }
    return !(h == 0 || w == 0);
}
void save_image(std::string file_name, std::uint32_t h, std::uint32_t w, const std::vector<double>& pixel_array)
{
    auto type = file_type(file_name);
    if(type == "") {
        std::cout << "File extension is missing!\n";
    } else if(type == "bmp") {
        file::bmp::write(file_name, h, w, pixel_array);
    } else if(type == "pgm") {
        file::pgm::write(file_name, h, w, pixel_array);
    } else if(type == "pbm") {
        file::pbm::write(file_name, h, w, pixel_array);
    } else {
        std::cout << "Unsupported file type!\n";
    }
}
image zero_pad(image in,unsigned int h1, unsigned int h2, unsigned int w1, unsigned int w2)
{
    image out(in.height()+h1+h2,in.width()+w1+w2);
    for(unsigned int i = h1; i < h1+in.height(); ++i) {
        std::copy(in.begin()+(i-h1)*in.width(),in.begin()+(i-h1)*in.width()+in.width(),out.begin()+i*out.width() + w1);
    }
    return out;
}
image copy_pad(image in,unsigned int h1, unsigned int h2, unsigned int w1, unsigned int w2)
{
    //Not implemented
    (void)h1;
    (void)h2;
    (void)w1;
    (void)w2;
    std::cout << "Not implemented.\n";
    return in;
}
image mirror_pad(image in,unsigned int h1, unsigned int h2, unsigned int w1, unsigned int w2)
{
    //Not implemented
    (void)h1;
    (void)h2;
    (void)w1;
    (void)w2;
    std::cout << "Not implemented.\n";
    return in;
}
image pad_image(image in,unsigned int h1, unsigned int h2, unsigned int w1, unsigned int w2, padding_type p)
{
    switch(p) {
        case padding_type::zero:
            return zero_pad(in,h1,h2,w1,w2);
        case padding_type::copy:
            return copy_pad(in,h1,h2,w1,w2);
        case padding_type::mirror:
            return mirror_pad(in,h1,h2,w1,w2);
    }
    return {0,0};
}
}
