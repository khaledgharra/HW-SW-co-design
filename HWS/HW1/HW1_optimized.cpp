#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <cstdint>

class LogDatabase {
private:
    std::vector<uint64_t> timestamps;
    std::vector<uint32_t> user_ids;
    std::vector<uint32_t> event_types;
    std::vector<std::array<char,64>> payloads;

public:
    explicit LogDatabase(size_t expected_size)
    {
        timestamps.reserve(expected_size);
        user_ids.reserve(expected_size);
        event_types.reserve(expected_size);
        payloads.reserve(expected_size);
    }

    void insert(
        uint64_t timestamp,
        uint32_t user_id,
        uint32_t event_type,
        const std::array<char,64>& payload)
    {
        timestamps.push_back(timestamp);
        user_ids.push_back(user_id);
        event_types.push_back(event_type);
        payloads.push_back(payload);
    }

    uint64_t count_user_events(uint32_t target_user) const
    {
        uint64_t count = 0;

        const uint32_t* data = user_ids.data();
        size_t n = user_ids.size();

        for (size_t i = 0; i < n; i++) {
            count += (data[i] == target_user);
        }

        return count;
    }
};

int main()
{
    constexpr size_t NUM_RECORDS = 20000000;
    constexpr size_t NUM_QUERIES = 100;

    LogDatabase db(NUM_RECORDS);

    std::mt19937 rng(42);

    std::uniform_int_distribution<uint32_t> user_dist(1, 10000);
    std::uniform_int_distribution<uint32_t> event_dist(1, 20);

    for (size_t i = 0; i < NUM_RECORDS; i++) {

        std::array<char,64> payload;

        for (int j = 0; j < 64; j++) {
            payload[j] = 'A' + (j % 26);
        }

        db.insert(
            i,
            user_dist(rng),
            event_dist(rng),
            payload);
    }

    uint64_t total = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t q = 0; q < NUM_QUERIES; q++) {
        total += db.count_user_events(user_dist(rng));
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Result: " << total << std::endl;
    std::cout << "Query Time: " << elapsed.count() << " seconds\n";
}