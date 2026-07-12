#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Simulates the external state store that FaaS functions read/write.
// In real FaaS (AWS Lambda, etc.) this would be DynamoDB / Redis.

enum class Shift    { MORNING, EVENING, NIGHT, OFF };
enum class Severity { LOW, MEDIUM, HIGH, CRITICAL };

struct DBPatient {
    int         id;
    std::string name;
    int         bed_id;
    int         dept_id;
    bool        admitted;
    double      bill_base;
};

struct DBDoctor {
    int         id;
    std::string name;
    int         dept_id;
    Shift       shift;
    bool        available;
};

struct DBNurse {
    int  id;
    int  dept_id;
    bool assigned;
};

struct DBOperatingRoom {
    int         id;
    bool        available;
    int         doctor_id;
    int         patient_id;
    std::string procedure;
};

struct DBEquipment {
    int         id;
    std::string type;
    bool        allocated;
    int         dept_id;
    int         patient_id;
};

struct DBBed {
    int  id;
    int  dept_id;
    bool occupied;
    int  patient_id;
};

struct DBEREntry {
    int         patient_id;
    Severity    severity;
    std::string symptoms;
};

// The whole hospital state lives here — passed by reference into every function.
struct Database {
    std::vector<DBPatient>       patients;
    std::vector<DBDoctor>        doctors;
    std::vector<DBNurse>         nurses;
    std::vector<DBOperatingRoom> or_rooms;
    std::vector<DBEquipment>     equipment;
    std::vector<DBBed>           beds;
    std::vector<DBEREntry>       er_queue;
};

// Factory — mirrors HospitalSystem constructor
inline Database make_database(int num_beds, int num_doctors,
                               int num_nurses, int num_ors, int num_equipment)
{
    Database db;
    for (int i = 0; i < num_beds; ++i)
        db.beds.push_back({i, i % 4, false, -1});
    for (int i = 0; i < num_doctors; ++i)
        db.doctors.push_back({i, "Doctor_" + std::to_string(i),
                              i % 4, Shift::OFF, true});
    for (int i = 0; i < num_nurses; ++i)
        db.nurses.push_back({i, -1, false});
    for (int i = 0; i < num_ors; ++i)
        db.or_rooms.push_back({i, true, -1, -1, ""});
    for (int i = 0; i < num_equipment; ++i)
        db.equipment.push_back({i, i % 2 == 0 ? "ventilator" : "monitor",
                                false, -1, -1});
    return db;
}
