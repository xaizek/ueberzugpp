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

#include "opencv.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

OpencvImage::OpencvImage(const Terminal& terminal,
        const std::string& filename, int max_width, int max_height,
        bool is_video):
terminal(terminal),
path(filename),
max_width(max_width * terminal.font_width),
max_height(max_height * terminal.font_height)
{
    if (is_video) {
        video = cv::VideoCapture(filename, cv::CAP_FFMPEG);
        video.read(image);
    } else {
        image = cv::imread(filename, cv::IMREAD_COLOR);
    }
    process_image();
}

auto OpencvImage::width() const-> int
{
    return _width;
}

auto OpencvImage::height() const -> int
{
    return _height;
}

auto OpencvImage::size() const -> unsigned long
{
    return _size;
}

auto OpencvImage::data() const -> const unsigned char*
{
    return image.ptr();
}

auto OpencvImage::framerate() const -> int
{
    if (video.isOpened()) return video.get(cv::CAP_PROP_FPS);
    return -1;
}

auto OpencvImage::next_frame() -> void
{
    // don't do anything unless opened by ffmpeg
    if (!video.isOpened()) return;
    if (!video.read(image)) {
        video.set(cv::CAP_PROP_POS_FRAMES, 0);
        video.read(image);
    }
    process_image();
}

auto OpencvImage::process_image() -> void
{
    _width = image.cols;
    _height = image.rows;

    unsigned long max_dim = (_width >= _height) ?
                            _width : _height;
    unsigned long new_width = 0, new_height = 0;
    double width_scale, height_scale, min_scale, max_scale;
    double scale = static_cast<double>(max_width) / max_dim;
    if (!(_width <= max_width && _height <= max_height)) {
        if (_width >= _height) {
            new_width = this->max_width;
            new_height = _height * scale;
        } else {
            new_height = this->max_width;
            new_width = _width * scale;
        }
    }

    if (new_width != 0 || new_height != 0) {
        cv::resize(image, image, cv::Size(new_width, new_height),
                0, 0, cv::INTER_AREA);
        _width = new_width;
        _height = new_height;
    }

    if (terminal.supports_sixel()) {
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_BGRA2RGB);
        } else {
            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        }
    } else {
        // alpha channel required
        cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
    }

    _size = image.total() * image.elemSize();
}
