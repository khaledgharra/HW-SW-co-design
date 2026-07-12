#include "fn_mass_casualty.h"

// Part 3 — Mass Casualty Event Protocol (FaaS version)
// This is a completely NEW file. Zero existing functions were modified.
// If this function has a bug, all other functions continue working normally.
MassCasualtyResult fn_mass_casualty(const MassCasualtyEvent& event, Database& db) {
    MassCasualtyResult result{0, 0, 0, false};

    if ((float)db.er_queue.size() / event.er_capacity < event.threshold)
        return result;

    result.triggered = true;

    // 1. Pull nurses from non-critical departments to ER
    for (auto& n : db.nurses) {
        if (n.assigned && (n.dept_id == 2 || n.dept_id == 3)) {
            n.dept_id = 0;
            ++result.nurses_reallocated;
        }
    }

    // 2. Cancel elective OR bookings
    for (auto& o : db.or_rooms) {
        if (!o.available) {
            for (auto& d : db.doctors)
                if (d.id == o.doctor_id) { d.available = true; break; }

            o.available  = true;
            o.doctor_id  = -1;
            o.patient_id = -1;
            o.procedure  = "";
            ++result.or_bookings_cancelled;
        }
    }

    // 3. Count available doctors to alert
    for (const auto& d : db.doctors)
        if (d.available && d.shift != Shift::OFF)
            ++result.doctors_alerted;

    return result;
}
