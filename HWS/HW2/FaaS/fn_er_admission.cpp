#include "fn_er_admission.h"
#include <algorithm>

ERResult fn_er_admission(const EREvent& event, Database& db) {
    db.er_queue.push_back({event.patient_id, event.severity, event.symptoms});

    std::sort(db.er_queue.begin(), db.er_queue.end(),
              [](const DBEREntry& a, const DBEREntry& b) {
                  return static_cast<int>(a.severity) > static_cast<int>(b.severity);
              });

    int pos = 0;
    for (int i = 0; i < (int)db.er_queue.size(); ++i)
        if (db.er_queue[i].patient_id == event.patient_id) { pos = i; break; }

    return {true, pos};
}
