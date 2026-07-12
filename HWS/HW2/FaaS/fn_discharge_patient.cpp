#include "fn_discharge_patient.h"

DischargeResult fn_discharge_patient(const DischargeEvent& event, Database& db) {
    DBPatient* patient = nullptr;
    for (auto& p : db.patients)
        if (p.id == event.patient_id && p.admitted) { patient = &p; break; }
    if (!patient) return {false};

    for (auto& b : db.beds)
        if (b.patient_id == event.patient_id) { b.occupied = false; b.patient_id = -1; break; }

    for (auto& e : db.equipment)
        if (e.patient_id == event.patient_id) { e.allocated = false; e.dept_id = -1; e.patient_id = -1; }

    patient->admitted = false;
    return {true};
}
