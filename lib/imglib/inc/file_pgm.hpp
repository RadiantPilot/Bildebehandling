/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _PGM_HEADER_
#define _PGM_HEADER_
#include <string>
#include <vector>
#include "file.hpp"
namespace imglib::file::pgm {
class header : public file::header
{
private:
    std::uint32_t WIDTH;
    std::uint32_t HEIGHT;
public:
    header() = default;
    header(std::uint32_t w, std::uint32_t h);
    header(std::string w, std::string h){parse_strings(w,h);}

    std::uint32_t width() const override {return WIDTH;}
    std::uint32_t height() const override {return HEIGHT;}

    void parse_strings(std::string w, std::string h);
    std::vector<std::string> get_strings() const;
};
header read(std::string file_name, std::vector<double> &pixel_table);
void write(std::string file_name, const header& head, const std::vector<double> &pixel_table);
void write(std::string file_name, std::uint32_t rows, std::uint32_t cols, const std::vector<double> &pixel_table);
} // namespace imglib::file::pgm
#endif
