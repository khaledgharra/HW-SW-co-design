#include "dispatcher.h"
#include "fn_admit_patient.h"
#include "fn_discharge_patient.h"
#include "fn_assign_doctor_shift.h"
#include "fn_assign_nurse.h"
#include "fn_schedule_or.h"
#include "fn_er_admission.h"
#include "fn_allocate_equipment.h"
#include "fn_generate_bill.h"
#include "fn_mass_casualty.h"
#include <any>

Dispatcher make_dispatcher() {
    Dispatcher d;

    d.register_function("admit_patient", [](const std::any& ev, Database& db) -> std::any {
        return fn_admit_patient(std::any_cast<AdmitEvent>(ev), db);
    });

    d.register_function("discharge_patient", [](const std::any& ev, Database& db) -> std::any {
        return fn_discharge_patient(std::any_cast<DischargeEvent>(ev), db);
    });

    d.register_function("assign_doctor_shift", [](const std::any& ev, Database& db) -> std::any {
        return fn_assign_doctor_shift(std::any_cast<ShiftEvent>(ev), db);
    });

    d.register_function("assign_nurse", [](const std::any& ev, Database& db) -> std::any {
        return fn_assign_nurse(std::any_cast<NurseEvent>(ev), db);
    });

    d.register_function("schedule_or", [](const std::any& ev, Database& db) -> std::any {
        return fn_schedule_or(std::any_cast<OREvent>(ev), db);
    });

    d.register_function("er_admission", [](const std::any& ev, Database& db) -> std::any {
        return fn_er_admission(std::any_cast<EREvent>(ev), db);
    });

    d.register_function("allocate_equipment", [](const std::any& ev, Database& db) -> std::any {
        return fn_allocate_equipment(std::any_cast<EquipEvent>(ev), db);
    });

    d.register_function("generate_bill", [](const std::any& ev, Database& db) -> std::any {
        return fn_generate_bill(std::any_cast<BillEvent>(ev), db);
    });

    // Part 3 — one new registration, zero existing functions changed
    d.register_function("mass_casualty", [](const std::any& ev, Database& db) -> std::any {
        return fn_mass_casualty(std::any_cast<MassCasualtyEvent>(ev), db);
    });

    return d;
}
