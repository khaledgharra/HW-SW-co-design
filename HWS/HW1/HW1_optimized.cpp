#include <iostream>
#include <vector>
#include <random>
#include <chrono>

struct LogEntry {
    uint64_t timestamp;
    uint32_t user_id;
    uint32_t event_type;
    char payload[64];
};

class LogDatabase {
private:
    std::vector<LogEntry> logs;

public:
    void insert(const LogEntry& entry) {
        logs.push_back(entry);
    }

    uint64_t count_user_events(uint32_t target_user) const {
        uint64_t count = 0;

        for (const auto& log : logs) {
            if (log.user_id == target_user) {
                count++;
            }
        }

        return count;
    }
};

int main() {
    constexpr size_t NUM_RECORDS = 200000;
    constexpr size_t NUM_QUERIES = 20;

    LogDatabase db;

    std::mt19937 rng(42);

    std::uniform_int_distribution<uint32_t> user_dist(1, 10000);
    std::uniform_int_distribution<uint32_t> event_dist(1, 20);

    std::cout << "Generating records..." << std::endl;

    for (size_t i = 0; i < NUM_RECORDS; i++) {
        LogEntry entry;

        entry.timestamp = i;
        entry.user_id = user_dist(rng);
        entry.event_type = event_dist(rng);

        for (int j = 0; j < 64; j++) {
            entry.payload[j] = static_cast<char>('A' + (j % 26));
        }

        db.insert(entry);
    }

    std::cout << "Running queries..." << std::endl;

    uint64_t total = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t q = 0; q < NUM_QUERIES; q++) {
        uint32_t target_user = user_dist(rng);
        total += db.count_user_events(target_user);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Result: " << total << std::endl;
    std::cout << "Query Time: "
              << elapsed.count()
              << " seconds" << std::endl;

    return 0;
}