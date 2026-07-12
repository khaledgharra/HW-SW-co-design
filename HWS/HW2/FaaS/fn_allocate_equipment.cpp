#include "fn_allocate_equipment.h"

EquipResult fn_allocate_equipment(const EquipEvent& event, Database& db) {
    for (auto& e : db.equipment) {
        if (e.id == event.equip_id) {
            if (e.allocated) return {false};
            e.allocated  = true;
            e.dept_id    = event.dept_id;
            e.patient_id = event.patient_id;
            return {true};
        }
    }
    return {false};
}
