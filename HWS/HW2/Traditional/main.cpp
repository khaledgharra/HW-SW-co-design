#include "hospital.h"
#include <iostream>
#include <chrono>
#include <string>

static constexpr int ITERATIONS   = 200;
static constexpr int NUM_BEDS      = 200;
static constexpr int NUM_DOCTORS   = 50;
static constexpr int NUM_NURSES    = 80;
static constexpr int NUM_ORS       = 20;
static constexpr int NUM_EQUIPMENT = 100;

int main() {
    auto wall_start = std::chrono::high_resolution_clock::now();

    long long ops = 0;

    for (int iter = 0; iter < ITERATIONS; ++iter) {
        HospitalSystem sys(NUM_BEDS, NUM_DOCTORS, NUM_NURSES,
                           NUM_ORS, NUM_EQUIPMENT);

        // 1. Admit patients
        for (int i = 0; i < 40; ++i) {
            sys.admitPatient(i, "Patient_" + std::to_string(i),
                             "dept_" + std::to_string(i % 4));
            ++ops;
        }

        // 2. Assign doctor shifts
        for (int i = 0; i < NUM_DOCTORS; ++i) {
            Shift s = static_cast<Shift>(i % 3);
            sys.assignDoctorShift(i, s);
            ++ops;
        }

        // 3. Assign nurses to departments
        for (int i = 0; i < NUM_NURSES; ++i) {
            sys.assignNurseToDept(i, i % 4);
            ++ops;
        }

        // 4. Schedule operating rooms
        for (int i = 0; i < 10; ++i) {
            sys.scheduleOR(i, i, i, "Procedure_" + std::to_string(i));
            ++ops;
        }

        // 5. ER admissions
        for (int i = 40; i < 60; ++i) {
            Severity sev = static_cast<Severity>(i % 4);
            sys.handleERAdmission(i, sev, "Symptom_" + std::to_string(i));
            ++ops;
        }

        // 6. Allocate equipment
        for (int i = 0; i < 30; ++i) {
            sys.allocateEquipment(i, i % 4, i % 40);
            ++ops;
        }

        // 7. Generate bills
        for (int i = 0; i < 40; ++i) {
            volatile Bill b = sys.generateBill(i);
            (void)b;
            ++ops;
        }

        // 8. Discharge patients
        for (int i = 0; i < 40; ++i) {
            sys.dischargePatient(i);
            ++ops;
        }
    }

    auto wall_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = wall_end - wall_start;

    std::cout << "[Traditional] ops=" << ops
              << "  time=" << elapsed.count() << "s\n";
    return 0;
}
