#pragma once
#include "events.h"
BillResult fn_generate_bill(const BillEvent& event, Database& db);
