#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>

constexpr int N = 8192;

int main()
{
    std::vector<uint32_t> input(N * N);
    std::vector<uint32_t> output(N * N);

    for (size_t i = 0; i < input.size(); ++i)
    {
        input[i] = static_cast<uint32_t>(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    constexpr int ITERATIONS = 4;
    for (int iter = 0; iter < ITERATIONS; iter++)
{
    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < N; ++x)
        {
            output[x * N + y] = input[y * N + x];
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