/*
 * SPDX-FileCopyrightText: 2026 Tom Stian Andersen <tom.s.andersen@uit.no>
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_
#include <cstdint>
namespace imglib::file {
class header
{
  public:
    virtual std::uint32_t width() const = 0;
    virtual std::uint32_t height() const = 0;
    virtual ~header() {};
};

} // namespace imglib::file
#endif
