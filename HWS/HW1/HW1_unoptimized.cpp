#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>

constexpr int WIDTH = 4096;
constexpr int HEIGHT = 4096;
constexpr int ITERATIONS = 50;

using Image = std::vector<std::vector<uint8_t>>;

void blur(const Image& input, Image& output)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int sum = 0;
            int count = 0;

            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int ny = y + dy;
                    int nx = x + dx;

                    if (ny >= 0 && ny < HEIGHT &&
                        nx >= 0 && nx < WIDTH)
                    {
                        sum += input[ny][nx];
                        count++;
                    }
                }
            }

            output[y][x] = static_cast<uint8_t>(sum / count);
        }
    }
}

int main()
{
    Image img(HEIGHT, std::vector<uint8_t>(WIDTH));
    Image tmp(HEIGHT, std::vector<uint8_t>(WIDTH));

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            img[y][x] = dist(rng);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
    {
        blur(img, tmp);
        std::swap(img, tmp);
    }

    auto end = std::chrono::high_resolution_clock::now();

    uint64_t checksum = 0;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            checksum += img[y][x];
        }
    }

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Checksum: " << checksum << "\n";
    std::cout << "Time: " << elapsed.count() << " sec\n";

    return 0;
}