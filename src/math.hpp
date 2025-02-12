#ifndef MATH_HPP
#define MATH_HPP

#include <complex>

#include "util/types.hpp"

using Complex = std::complex<f32>;

constexpr static f32 pi = 3.1415926535897;

void fft(Complex in[], Complex out[], u32 stride, u32 N);

void dft(Complex nums[], Complex out[], u32 N);

#endif // MATH_HPP
