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

#ifndef __APPLICATION__
#define __APPLICATION__

#include "image.hpp"
#include "canvas.hpp"
#include "terminal.hpp"
#include "flags.hpp"
#include "dimensions.hpp"

#include <string>
#include <memory>
#include <atomic>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

class Application
{
public:
    Application(const Flags& flags);
    ~Application();

    void execute(const std::string& cmd);
    void command_loop(const std::atomic<bool>& flag);

private:
    Terminal terminal;
    std::unique_ptr<Dimensions> dimensions;

    std::shared_ptr<Image> image;
    std::unique_ptr<Canvas> canvas;
    std::shared_ptr<spdlog::logger> logger;
    std::FILE* f_stderr = nullptr;
    const Flags& flags;

    auto setup_logger() -> void;
    auto set_silent() -> void;
    auto print_header() -> void;
    auto tcp_loop(const std::atomic<bool>& stop_flag) -> void;
    void set_dimensions_from_json(const nlohmann::json& json);
};

#endif
