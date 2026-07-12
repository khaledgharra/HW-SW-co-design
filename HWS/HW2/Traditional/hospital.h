#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class Shift   { MORNING, EVENING, NIGHT, OFF };
enum class Severity { LOW, MEDIUM, HIGH, CRITICAL };

struct Patient {
    int         id;
    std::string name;
    int         bed_id;
    std::string department;
    bool        admitted;
    double      bill_base;
};

struct Doctor {
    int         id;
    std::string name;
    std::string department;
    Shift       shift;
    bool        available;
};

struct Nurse {
    int         id;
    std::string name;
    int         dept_id;
    bool        assigned;
};

struct OperatingRoom {
    int         id;
    bool        available;
    int         doctor_id;
    int         patient_id;
    std::string procedure;
};

struct Equipment {
    int         id;
    std::string type;
    bool        allocated;
    int         dept_id;
    int         patient_id;
};

struct Bed {
    int         id;
    int         dept_id;
    bool        occupied;
    int         patient_id;
};

struct EREntry {
    int         patient_id;
    Severity    severity;
    std::string symptoms;
};

struct Bill {
    int    patient_id;
    double base;
    double equipment_fee;
    double doctor_fee;
    double total;
};

class HospitalSystem {
public:
    HospitalSystem(int num_beds, int num_doctors, int num_nurses,
                   int num_ors, int num_equipment);

    // 8 operations
    bool  admitPatient(int patient_id, const std::string& name, const std::string& dept);
    bool  dischargePatient(int patient_id);
    bool  assignDoctorShift(int doctor_id, Shift shift);
    bool  assignNurseToDept(int nurse_id, int dept_id);
    bool  scheduleOR(int or_id, int doctor_id, int patient_id, const std::string& proc);
    bool  handleERAdmission(int patient_id, Severity severity, const std::string& symptoms);
    bool  allocateEquipment(int equip_id, int dept_id, int patient_id);
    Bill  generateBill(int patient_id);

    int   availableBeds(int dept_id) const;
    int   erQueueSize()              const;

private:
    std::vector<Patient>      patients_;
    std::vector<Doctor>       doctors_;
    std::vector<Nurse>        nurses_;
    std::vector<OperatingRoom> or_rooms_;
    std::vector<Equipment>    equipment_;
    std::vector<Bed>          beds_;
    std::vector<EREntry>      er_queue_;

    Patient*       findPatient(int id);
    Doctor*        findDoctor(int id);
    Nurse*         findNurse(int id);
    OperatingRoom* findOR(int id);
    Equipment*     findEquipment(int id);
    int            findFreeBed(int dept_id);
};
