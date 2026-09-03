/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _IMAGE_
#define _IMAGE_
#include <string>
#include <vector>
namespace imglib {
class image
{
  protected:
    unsigned int w;
    unsigned int h;
    std::vector<double> pixel_array;
  public:
    image() = delete;
    image(std::string file_name);
    image(unsigned int rows, unsigned int cols);

    double &operator()(unsigned int row, unsigned int col);
    double operator()(unsigned int row, unsigned int col) const;

    std::vector<double>::iterator begin();
    std::vector<double>::iterator end();
    void save(std::string file_name);
    unsigned int width() const { return w; }
    unsigned int height() const { return h; }
    void clear()
    {
        std::fill(pixel_array.begin(),pixel_array.end(),0.0);
    }
};
inline double &image::operator()(unsigned int row, unsigned int col)
{
    return pixel_array[w * row + col];
}
inline double image::operator()(unsigned int row, unsigned int col) const
{
    return pixel_array.at(w * row + col);
}
} // namespace imglib
#endif
