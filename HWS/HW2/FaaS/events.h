#pragma once
#include "db.h"
#include <string>

// Every FaaS function is triggered by an event and returns a result.
// No function holds state between invocations.

struct AdmitEvent   { int patient_id; std::string name; int dept_id; };
struct AdmitResult  { bool success; int bed_assigned; };

struct DischargeEvent  { int patient_id; };
struct DischargeResult { bool success; };

struct ShiftEvent  { int doctor_id; Shift shift; };
struct ShiftResult { bool success; };

struct NurseEvent  { int nurse_id; int dept_id; };
struct NurseResult { bool success; };

struct OREvent  { int or_id; int doctor_id; int patient_id; std::string procedure; };
struct ORResult { bool success; };

struct EREvent  { int patient_id; Severity severity; std::string symptoms; };
struct ERResult { bool success; int queue_position; };

struct EquipEvent  { int equip_id; int dept_id; int patient_id; };
struct EquipResult { bool success; };

struct BillEvent  { int patient_id; };
struct BillResult { double base; double equip_fee; double doctor_fee; double total; };

// Part 3 — Mass Casualty Event Protocol
struct MassCasualtyEvent  { int er_capacity; float threshold; };
struct MassCasualtyResult { int nurses_reallocated; int or_bookings_cancelled;
                            int doctors_alerted;    bool triggered; };
