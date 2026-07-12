#include "fn_generate_bill.h"

BillResult fn_generate_bill(const BillEvent& event, Database& db) {
    const DBPatient* patient = nullptr;
    for (const auto& p : db.patients)
        if (p.id == event.patient_id) { patient = &p; break; }
    if (!patient) return {0, 0, 0, 0};

    double equip_fee = 0;
    for (const auto& e : db.equipment)
        if (e.patient_id == event.patient_id) equip_fee += 200.0;

    double doc_fee = 300.0;
    double total   = patient->bill_base + equip_fee + doc_fee;
    return {patient->bill_base, equip_fee, doc_fee, total};
}
