#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <algorithm>

constexpr int WIDTH = 4096;
constexpr int HEIGHT = 4096;
constexpr int ITERATIONS = 50;

// Optimized spatial convolution utilizing streaming pointers and loop vectorization
void blur(const std::vector<uint8_t>& input, std::vector<uint8_t>& output)
{
    const uint8_t* __restrict src = input.data();
    uint8_t* __restrict dst = output.data();

    // Process row by row to maintain perfect sequential memory strides (Stride = 1)
    for (int y = 1; y < HEIGHT - 1; ++y)
    {
        // Pre-compute base row offsets outside the innermost loop
        int row_curr = y * WIDTH;
        int row_prev = (y - 1) * WIDTH;
        int row_next = (y + 1) * WIDTH;

        // Force compiler auto-vectorization hint for the sequential row strip
        
        #pragma GCC ivdep
        for (int x = 1; x < WIDTH - 1; ++x)
        {
            // Spatial data streaming: Accesses are closely packed inside cache lines
            int sum =
                src[row_prev + x - 1] + src[row_prev + x] + src[row_prev + x + 1] +
                src[row_curr + x - 1] + src[row_curr + x] + src[row_curr + x + 1] +
                src[row_next + x - 1] + src[row_next + x] + src[row_next + x + 1];

            dst[row_curr + x] = static_cast<uint8_t>(sum / 9);
        }
    }
}

int main()
{
    std::vector<uint8_t> img(WIDTH * HEIGHT);
    std::vector<uint8_t> tmp(WIDTH * HEIGHT);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);

    for (auto& p : img) { p = dist(rng); }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
    {
        blur(img, tmp);
        std::swap(img, tmp);
    }

    auto end = std::chrono::high_resolution_clock::now();
    uint64_t checksum = 0;
    for (auto p : img) { checksum += p; }

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "Time: " << elapsed.count() << " sec\n";

    return 0;
}