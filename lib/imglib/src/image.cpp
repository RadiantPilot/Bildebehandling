/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#include "image.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
namespace imglib {
image::image(std::string file_name) {
  if (!read_image(file_name, h, w, pixel_array)) {
    std::cout << "Failed to open image!\nContructing (" << 10 << ", " << 10
              << ") smiley face instead :D save image view it." << '\n';
    h = 10;
    w = 10;
    pixel_array = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,   0,
                   255, 255, 0,   0,   255, 255, 255, 255, 0,   0,   255, 255,
                   0,   0,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                   255, 255, 255, 0,   255, 255, 255, 255, 255, 255, 0,   255,
                   255, 255, 0,   0,   0,   0,   0,   0,   255, 255, 255, 255,
                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                   255, 255, 255, 255};
  }
}
std::vector<double>::iterator image::begin()
{
  return pixel_array.begin();
}
std::vector<double>::iterator image::end()
{
  return pixel_array.end();
}
image::image(unsigned int rows, unsigned int cols) {
  h = rows;
  w = cols;
  pixel_array.resize(h * w);
}

void image::save(std::string file_name) {
  save_image(file_name, h, w, pixel_array);
}
} // namespace imglib
