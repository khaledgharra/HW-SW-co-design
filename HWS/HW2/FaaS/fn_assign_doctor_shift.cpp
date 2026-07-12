#include "fn_assign_doctor_shift.h"

ShiftResult fn_assign_doctor_shift(const ShiftEvent& event, Database& db) {
    for (auto& d : db.doctors) {
        if (d.id == event.doctor_id) {
            d.shift     = event.shift;
            d.available = (event.shift != Shift::OFF);
            return {true};
        }
    }
    return {false};
}
