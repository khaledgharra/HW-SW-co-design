#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <algorithm>

constexpr int WIDTH = 4096;
constexpr int HEIGHT = 4096;
constexpr int ITERATIONS = 50;
constexpr int TILE = 64;

inline int idx(int y, int x)
{
    return y * WIDTH + x;
}

void blur(const std::vector<uint8_t>& input,
          std::vector<uint8_t>& output)
{
    for (int by = 1; by < HEIGHT - 1; by += TILE)
    {
        for (int bx = 1; bx < WIDTH - 1; bx += TILE)
        {
            int y_end = std::min(by + TILE, HEIGHT - 1);
            int x_end = std::min(bx + TILE, WIDTH - 1);

            for (int y = by; y < y_end; y++)
            {
                for (int x = bx; x < x_end; x++)
                {
                    int center = idx(y, x);

                    int sum =
                        input[center - WIDTH - 1] +
                        input[center - WIDTH] +
                        input[center - WIDTH + 1] +
                        input[center - 1] +
                        input[center] +
                        input[center + 1] +
                        input[center + WIDTH - 1] +
                        input[center + WIDTH] +
                        input[center + WIDTH + 1];

                    output[center] =
                        static_cast<uint8_t>(sum / 9);
                }
            }
        }
    }
}

int main()
{
    std::vector<uint8_t> img(WIDTH * HEIGHT);
    std::vector<uint8_t> tmp(WIDTH * HEIGHT);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);

    for (auto& p : img)
    {
        p = dist(rng);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
    {
        blur(img, tmp);
        std::swap(img, tmp);
    }

    auto end = std::chrono::high_resolution_clock::now();

    uint64_t checksum = 0;

    for (auto p : img)
    {
        checksum += p;
    }

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "Time: " << elapsed.count() << " sec\n";

    return 0;
}