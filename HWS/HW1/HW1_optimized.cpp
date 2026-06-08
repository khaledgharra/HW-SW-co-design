#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <algorithm>

constexpr int N = 8192;
constexpr int TILE = 64;
constexpr int ITERATIONS = 4;

int main()
{
    std::vector<uint32_t> input(N * N);
    std::vector<uint32_t> output(N * N);

    for (size_t i = 0; i < input.size(); ++i)
    {
        input[i] = static_cast<uint32_t>(i);
    }

    auto start = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();

for (int iter = 0; iter < ITERATIONS; iter++)
{
    for (int by = 0; by < N; by += TILE)
    {
        for (int bx = 0; bx < N; bx += TILE)
        {
            int y_end = std::min(by + TILE, N);
            int x_end = std::min(bx + TILE, N);

            for (int y = by; y < y_end; ++y)
            {
                const int in_row = y * N;

                for (int x = bx; x < x_end; ++x)
                {
                    output[x * N + y] =
                        input[in_row + x];
                }
            }
        }
    }

    std::swap(input, output);
}

auto end = std::chrono::high_resolution_clock::now();

    uint64_t checksum = 0;

    for (auto val : output)
    {
        checksum += val;
    }

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Checksum: " << checksum << std::endl;
    std::cout << "Time: " << elapsed.count() << " sec" << std::endl;

    return 0;
}