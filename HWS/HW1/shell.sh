g++ -O2 -std=c++17 HW1_unoptimized.cpp -o HW1_unoptimized
g++ -O2 -std=c++17 HW1_optimized.cpp -o HW1_optimized
./HW1_optimized
./HW1_unoptimized
perf stat ./HW1_unoptimized
perf stat ./HW1_optimized