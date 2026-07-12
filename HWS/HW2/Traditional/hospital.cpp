#include "hospital.h"
#include <stdexcept>
#include <algorithm>

HospitalSystem::HospitalSystem(int num_beds, int num_doctors, int num_nurses,
                               int num_ors, int num_equipment)
{
    // Initialise beds across 4 departments (IDs 0-3)
    for (int i = 0; i < num_beds; ++i)
        beds_.push_back({i, i % 4, false, -1});

    for (int i = 0; i < num_doctors; ++i)
        doctors_.push_back({i, "Doctor_" + std::to_string(i),
                            "dept_" + std::to_string(i % 4),
                            Shift::OFF, true});

    for (int i = 0; i < num_nurses; ++i)
        nurses_.push_back({i, "Nurse_" + std::to_string(i), -1, false});

    for (int i = 0; i < num_ors; ++i)
        or_rooms_.push_back({i, true, -1, -1, ""});

    for (int i = 0; i < num_equipment; ++i)
        equipment_.push_back({i, i % 2 == 0 ? "ventilator" : "monitor",
                              false, -1, -1});
}

// ── helpers ──────────────────────────────────────────────────────────────────

Patient* HospitalSystem::findPatient(int id) {
    for (auto& p : patients_)
        if (p.id == id) return &p;
    return nullptr;
}
Doctor* HospitalSystem::findDoctor(int id) {
    for (auto& d : doctors_)
        if (d.id == id) return &d;
    return nullptr;
}
Nurse* HospitalSystem::findNurse(int id) {
    for (auto& n : nurses_)
        if (n.id == id) return &n;
    return nullptr;
}
OperatingRoom* HospitalSystem::findOR(int id) {
    for (auto& o : or_rooms_)
        if (o.id == id) return &o;
    return nullptr;
}
Equipment* HospitalSystem::findEquipment(int id) {
    for (auto& e : equipment_)
        if (e.id == id) return &e;
    return nullptr;
}
int HospitalSystem::findFreeBed(int dept_id) {
    for (auto& b : beds_)
        if (b.dept_id == dept_id && !b.occupied) return b.id;
    return -1;
}

// ── operations ───────────────────────────────────────────────────────────────

bool HospitalSystem::admitPatient(int patient_id, const std::string& name,
                                  const std::string& dept)
{
    if (findPatient(patient_id)) return false;   // already admitted

    int dept_id = std::stoi(dept.substr(5));     // "dept_X"
    int bed = findFreeBed(dept_id);
    if (bed < 0) return false;

    beds_[bed].occupied  = true;
    beds_[bed].patient_id = patient_id;
    patients_.push_back({patient_id, name, bed, dept, true, 500.0});
    return true;
}

bool HospitalSystem::dischargePatient(int patient_id) {
    Patient* p = findPatient(patient_id);
    if (!p || !p->admitted) return false;

    // free bed
    for (auto& b : beds_)
        if (b.patient_id == patient_id) { b.occupied = false; b.patient_id = -1; break; }

    // release equipment
    for (auto& e : equipment_)
        if (e.patient_id == patient_id) { e.allocated = false; e.dept_id = -1; e.patient_id = -1; }

    p->admitted = false;
    return true;
}

bool HospitalSystem::assignDoctorShift(int doctor_id, Shift shift) {
    Doctor* d = findDoctor(doctor_id);
    if (!d) return false;
    d->shift     = shift;
    d->available = (shift != Shift::OFF);
    return true;
}

bool HospitalSystem::assignNurseToDept(int nurse_id, int dept_id) {
    Nurse* n = findNurse(nurse_id);
    if (!n) return false;
    n->dept_id  = dept_id;
    n->assigned = true;
    return true;
}

bool HospitalSystem::scheduleOR(int or_id, int doctor_id, int patient_id,
                                const std::string& proc)
{
    OperatingRoom* o = findOR(or_id);
    Doctor*        d = findDoctor(doctor_id);
    Patient*       p = findPatient(patient_id);
    if (!o || !d || !p)           return false;
    if (!o->available)            return false;
    if (!d->available)            return false;

    o->available  = false;
    o->doctor_id  = doctor_id;
    o->patient_id = patient_id;
    o->procedure  = proc;
    d->available  = false;
    return true;
}

bool HospitalSystem::handleERAdmission(int patient_id, Severity severity,
                                       const std::string& symptoms)
{
    er_queue_.push_back({patient_id, severity, symptoms});

    // sort queue: CRITICAL first
    std::sort(er_queue_.begin(), er_queue_.end(),
              [](const EREntry& a, const EREntry& b) {
                  return static_cast<int>(a.severity) > static_cast<int>(b.severity);
              });
    return true;
}

bool HospitalSystem::allocateEquipment(int equip_id, int dept_id, int patient_id) {
    Equipment* e = findEquipment(equip_id);
    if (!e || e->allocated) return false;
    e->allocated  = true;
    e->dept_id    = dept_id;
    e->patient_id = patient_id;
    return true;
}

Bill HospitalSystem::generateBill(int patient_id) {
    Patient* p = findPatient(patient_id);
    if (!p) return {patient_id, 0, 0, 0, 0};

    double equip_fee = 0;
    for (const auto& e : equipment_)
        if (e.patient_id == patient_id) equip_fee += 200.0;

    double doc_fee = 300.0;
    double total   = p->bill_base + equip_fee + doc_fee;
    return {patient_id, p->bill_base, equip_fee, doc_fee, total};
}

int HospitalSystem::availableBeds(int dept_id) const {
    int count = 0;
    for (const auto& b : beds_)
        if (b.dept_id == dept_id && !b.occupied) ++count;
    return count;
}

int HospitalSystem::erQueueSize() const {
    return static_cast<int>(er_queue_.size());
}
