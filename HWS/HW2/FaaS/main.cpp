#include "dispatcher.h"
#include "db.h"
#include "events.h"
#include <iostream>
#include <chrono>
#include <string>

static constexpr int ITERATIONS    = 5000;
static constexpr int NUM_BEDS      = 200;
static constexpr int NUM_DOCTORS   = 50;
static constexpr int NUM_NURSES    = 80;
static constexpr int NUM_ORS       = 20;
static constexpr int NUM_EQUIPMENT = 100;

int main() {
    Dispatcher dispatcher = make_dispatcher();
    auto wall_start = std::chrono::high_resolution_clock::now();

    long long ops = 0;

    for (int iter = 0; iter < ITERATIONS; ++iter) {
        Database db = make_database(NUM_BEDS, NUM_DOCTORS, NUM_NURSES,
                                    NUM_ORS, NUM_EQUIPMENT);

        // 1. Admit patients — each is an independent function invocation
        for (int i = 0; i < 40; ++i) {
            AdmitEvent ev{i, "Patient_" + std::to_string(i), i % 4};
            dispatcher.invoke("admit_patient", ev, db);
            ++ops;
        }

        // 2. Assign doctor shifts
        for (int i = 0; i < NUM_DOCTORS; ++i) {
            ShiftEvent ev{i, static_cast<Shift>(i % 3)};
            dispatcher.invoke("assign_doctor_shift", ev, db);
            ++ops;
        }

        // 3. Assign nurses
        for (int i = 0; i < NUM_NURSES; ++i) {
            NurseEvent ev{i, i % 4};
            dispatcher.invoke("assign_nurse", ev, db);
            ++ops;
        }

        // 4. Schedule operating rooms
        for (int i = 0; i < 10; ++i) {
            OREvent ev{i, i, i, "Procedure_" + std::to_string(i)};
            dispatcher.invoke("schedule_or", ev, db);
            ++ops;
        }

        // 5. ER admissions
        for (int i = 40; i < 60; ++i) {
            EREvent ev{i, static_cast<Severity>(i % 4),
                       "Symptom_" + std::to_string(i)};
            dispatcher.invoke("er_admission", ev, db);
            ++ops;
        }

        // 6. Allocate equipment
        for (int i = 0; i < 30; ++i) {
            EquipEvent ev{i, i % 4, i % 40};
            dispatcher.invoke("allocate_equipment", ev, db);
            ++ops;
        }

        // 7. Generate bills
        for (int i = 0; i < 40; ++i) {
            BillEvent ev{i};
            volatile auto r = std::any_cast<BillResult>(
                dispatcher.invoke("generate_bill", ev, db));
            (void)r;
            ++ops;
        }

        // 8. Discharge patients
        for (int i = 0; i < 40; ++i) {
            DischargeEvent ev{i};
            dispatcher.invoke("discharge_patient", ev, db);
            ++ops;
        }
    }

    auto wall_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = wall_end - wall_start;

    std::cout << "[FaaS] ops=" << ops
              << "  time=" << elapsed.count() << "s\n";
    return 0;
}
