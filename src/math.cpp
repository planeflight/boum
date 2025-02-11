#include "math.hpp"

// taken from: https://rosettacode.org/wiki/Fast_Fourier_transform#C++
void fft(std::valarray<Complex> &nums) {
    const u32 n = nums.size();
    if (n <= 1) return;
    std::valarray<Complex> even = nums[std::slice(0, n / 2, 2)];
    std::valarray<Complex> odd = nums[std::slice(1, n / 2, 2)];

    fft(even);
    fft(odd);

    // combine
    for (u32 k = 0; k < n / 2; ++k) {
        Complex t = std::polar(1.0f, -2.0f * pi * k / n) * odd[k];
        nums[k] = even[k] + t;
        nums[k + n / 2] = even[k] - t;
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
