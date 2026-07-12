#!/usr/bin/env bash
set -e

BASE="$(cd "$(dirname "$0")" && pwd)"
TRAD="$BASE/Traditional"
FAAS="$BASE/FaaS"
OUT="$BASE/results"
FG="$BASE/FlameGraph"

mkdir -p "$OUT"

# ── 0. Clone FlameGraph if missing ───────────────────────────────────────────
if [ ! -d "$FG" ]; then
    echo "[*] Cloning FlameGraph toolkit..."
    git clone https://github.com/brendangregg/FlameGraph "$FG"
fi

# ── 1. Compile ────────────────────────────────────────────────────────────────
echo "[*] Compiling Traditional..."
g++ -O0 -g -fno-omit-frame-pointer -std=c++17 \
    "$TRAD/hospital.cpp" "$TRAD/main.cpp" \
    -o "$OUT/traditional"

echo "[*] Compiling FaaS..."
g++ -O0 -g -fno-omit-frame-pointer -std=c++17 \
    "$FAAS/fn_admit_patient.cpp"     \
    "$FAAS/fn_discharge_patient.cpp" \
    "$FAAS/fn_assign_doctor_shift.cpp" \
    "$FAAS/fn_assign_nurse.cpp"      \
    "$FAAS/fn_schedule_or.cpp"       \
    "$FAAS/fn_er_admission.cpp"      \
    "$FAAS/fn_allocate_equipment.cpp" \
    "$FAAS/fn_generate_bill.cpp"     \
    "$FAAS/dispatcher.cpp"           \
    "$FAAS/main.cpp"                 \
    -o "$OUT/faas"

# ── 2. Quick smoke test ───────────────────────────────────────────────────────
echo ""
echo "[*] Running both binaries..."
"$OUT/traditional"
"$OUT/faas"

# ── 3. perf stat ─────────────────────────────────────────────────────────────
echo ""
echo "[*] perf stat — Traditional..."
perf stat -e cycles,instructions,cache-misses,cache-references,\
L1-dcache-load-misses,context-switches,task-clock \
    "$OUT/traditional" 2>&1 | tee "$OUT/perf_stat_traditional.txt"

echo ""
echo "[*] perf stat — FaaS..."
perf stat -e cycles,instructions,cache-misses,cache-references,\
L1-dcache-load-misses,context-switches,task-clock \
    "$OUT/faas" 2>&1 | tee "$OUT/perf_stat_faas.txt"

# ── 4. Flame graphs ──────────────────────────────────────────────────────────
echo ""
echo "[*] Recording FlameGraph — Traditional..."
perf record -e cpu-clock --call-graph fp -F 99 -o "$OUT/perf_traditional.data" "$OUT/traditional"
perf script -i "$OUT/perf_traditional.data" \
    | "$FG/stackcollapse-perf.pl" \
    | "$FG/flamegraph.pl" --title "Traditional Hospital System" \
    > "$OUT/flame_traditional.svg"

echo "[*] Recording FlameGraph — FaaS..."
perf record -e cpu-clock --call-graph fp -F 99 -o "$OUT/perf_faas.data" "$OUT/faas"
perf script -i "$OUT/perf_faas.data" \
    | "$FG/stackcollapse-perf.pl" \
    | "$FG/flamegraph.pl" --title "FaaS Hospital System" \
    > "$OUT/flame_faas.svg"

echo ""
echo "=== Done! Results in $OUT/ ==="
echo "  perf_stat_traditional.txt"
echo "  perf_stat_faas.txt"
echo "  flame_traditional.svg"
echo "  flame_faas.svg"
