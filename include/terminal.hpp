// Display images inside a terminal
// Copyright (C) 2023  JustKidding
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef __TERMINAL__
#define __TERMINAL__

#include "flags.hpp"
#include "util/x11.hpp"

#include <string>
#include <termios.h>

class Terminal
{
public:
    Terminal(int pid, const Flags& flags);
    ~Terminal();

    double font_width;
    double font_height;
    std::string name;

    auto supports_sixel() const -> bool;

private:
    auto get_terminal_size() -> void;
    auto guess_padding(int chars, double pixels) -> double;
    auto guess_font_size(int chars, double pixels, double padding) -> double;

    auto init_termios() -> void;
    auto reset_termios() -> void;
    auto get_terminal_size_escape_code() -> void;
    void get_terminal_size_pixels_fallback();
    void open_first_pty();

    int pty_fd;
    int pid;
    double padding_horizontal;
    double padding_vertical;
    int rows;
    int cols;
    int xpixel;
    int ypixel;

    const Flags& flags;

    struct termios old_term;
    struct termios new_term;

    const X11Util xutil;
};

#endif
