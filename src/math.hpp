#ifndef MATH_HPP
#define MATH_HPP

#include <complex>
#include <valarray>

#include "util/types.hpp"

using Complex = std::complex<f32>;

constexpr static f32 pi = 3.1415926535897;

void fft(std::valarray<Complex> &nums);

void dft(Complex nums[], Complex out[], u32 N);

#endif // MATH_HPP
