#ifndef BOUM_VIDEO_HPP
#define BOUM_VIDEO_HPP

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#include "util/types.hpp"

struct Video {
    FILE *ffmpeg_handle = nullptr;
    u32 w = 1600, h = 900;
    Video(u32 w, u32 h) : w(w), h(h) {}

    void ffmpeg_start(const std::string &sound_track,
                      const std::string &out,
                      u32 fps) {
        std::cout << "yo\n";
        std::stringstream cmd;
        cmd << "ffmpeg -loglevel verbose -y -f rawvideo -pix_fmt rgba -s ";
        cmd << w << "x" << h << " ";
        cmd << "-r " << fps << " -i - ";
        cmd << "-i " << sound_track
            << " -c:v libx264 -vb 2500k -c:a aac -ab 200k -pix_fmt yuv420p ";
        cmd << out;

        ffmpeg_handle = popen(cmd.str().c_str(), "w");
        if (!ffmpeg_handle) {
            std::cerr << "ERROR: Failed to open FFMPEG with the following "
                         "command: '"
                      << cmd.str() << "'\n";
        }
    }

    void write_frame(u32 *data) {
        fwrite(data, sizeof(u32), w * h, ffmpeg_handle);
    }

    void ffmpeg_end() {
        fflush(ffmpeg_handle);
        pclose(ffmpeg_handle);
    }
};

#endif // BOUM_VIDEO_HPP
