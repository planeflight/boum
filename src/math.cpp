#include "math.hpp"

// taken from: https://rosettacode.org/wiki/Fast_Fourier_transform#C++
void fft(Complex in[], Complex out[], u32 stride, u32 N) {
    if (N <= 1) {
        out[0] = in[0];
        return;
    }

    // even/odd calcs
    Complex even[N / 2], odd[N / 2];
    fft(in, even, stride * 2, N / 2);
    fft(in + stride, odd, stride * 2, N / 2);

    // combine
    for (u32 k = 0; k < N / 2; ++k) {
        Complex t = std::polar(1.0f, -2.0f * pi * k / N) * odd[k];
        out[k] = even[k] + t;
        out[k + N / 2] = even[k] - t;
    }
}

void dft(Complex nums[], Complex out[], u32 N) {
    for (u32 k = 0; k < N; ++k) {
        out[k] = 0;
        for (u32 n = 0; n < N; ++n) {
            out[k] += nums[n] * std::polar(1.0f, -2.0f * pi * k * n / N);
        }
    }
}
