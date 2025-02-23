#ifndef BOUM_VIDEO_HPP
#define BOUM_VIDEO_HPP

#include <cstdio>
#include <iostream>
#include <string>

#include "util/types.hpp"

struct Video {
    FILE *ffmpeg_handle = nullptr;
    u32 w = 1920, h = 1080;
    Video(u32 w, u32 h) : w(w), h(h) {}

    void ffmpeg_start(const std::string &out, u32 fps) {
        ffmpeg_handle = popen(
            "ffmpeg -loglevel verbose -y -f rawvideo -pix_fmt rgba -s 1600x900 "
            "-r 60 -i - -c:v libx264 -vb 2500k -c:a aac -ab 200k "
            "-pix_fmt yuv420p output.mp4",
            "w");
        if (!ffmpeg_handle) {
            std::cerr << "ERROR: Failed to open FFMPEG pipe\n";
        }
    }

    void write_frame(u32 *data) {
        fwrite(data, sizeof(u32), w * h, ffmpeg_handle);
    }

    void ffmpeg_end() {
        pclose(ffmpeg_handle);
    }
};

#endif // BOUM_VIDEO_HPP
