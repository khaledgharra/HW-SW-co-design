#include "fn_assign_nurse.h"

NurseResult fn_assign_nurse(const NurseEvent& event, Database& db) {
    for (auto& n : db.nurses) {
        if (n.id == event.nurse_id) {
            n.dept_id  = event.dept_id;
            n.assigned = true;
            return {true};
        }
    }
    return {false};
}
