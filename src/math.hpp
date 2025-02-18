#ifndef MATH_HPP
#define MATH_HPP

#include <raylib.h>

#include <complex>

#include "util/types.hpp"

using Complex = std::complex<f32>;

constexpr static f32 pi = 3.1415926535897;

void fft(Complex in[], Complex out[], u32 stride, u32 N);

void dft(Complex nums[], Complex out[], u32 N);

inline Color hsv_to_rgb(f32 h, f32 s, f32 v) {
    float c = v * s;
    float x = c * (1.0f - fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r, g, b;
    if (h < 60.0f) {
        r = c, g = x, b = 0.0f;
    } else if (h < 120.0f) {
        r = x, g = c, b = 0.0f;
    } else if (h < 180.0f) {
        r = 0, g = c, b = x;
    } else if (h < 240.0f) {
        r = 0, g = x, b = c;
    } else if (h < 300.0f) {
        r = x, g = 0.0f, b = c;
    } else {
        r = c, g = 0.0f, b = x;
    }

    return Color{
        (u8)((r + m) * 255), (u8)((g + m) * 255), (u8)((b + m) * 255), 255};
}

#endif // MATH_HPP
