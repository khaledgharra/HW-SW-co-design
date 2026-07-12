#include "fn_schedule_or.h"

ORResult fn_schedule_or(const OREvent& event, Database& db) {
    DBOperatingRoom* room   = nullptr;
    DBDoctor*        doctor = nullptr;
    bool patient_found      = false;

    for (auto& o : db.or_rooms)
        if (o.id == event.or_id)     { room   = &o; break; }
    for (auto& d : db.doctors)
        if (d.id == event.doctor_id) { doctor = &d; break; }
    for (const auto& p : db.patients)
        if (p.id == event.patient_id && p.admitted) { patient_found = true; break; }

    if (!room || !doctor || !patient_found) return {false};
    if (!room->available || !doctor->available) return {false};

    room->available  = false;
    room->doctor_id  = event.doctor_id;
    room->patient_id = event.patient_id;
    room->procedure  = event.procedure;
    doctor->available = false;
    return {true};
}
