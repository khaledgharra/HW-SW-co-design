#include "fn_admit_patient.h"

AdmitResult fn_admit_patient(const AdmitEvent& event, Database& db) {
    // stateless: scan DB for duplicate
    for (const auto& p : db.patients)
        if (p.id == event.patient_id && p.admitted)
            return {false, -1};

    // find free bed in requested department
    int bed_id = -1;
    for (auto& b : db.beds) {
        if (b.dept_id == event.dept_id && !b.occupied) {
            bed_id = b.id;
            b.occupied   = true;
            b.patient_id = event.patient_id;
            break;
        }
    }
    if (bed_id < 0) return {false, -1};

    db.patients.push_back({event.patient_id, event.name,
                            bed_id, event.dept_id, true, 500.0});
    return {true, bed_id};
}
