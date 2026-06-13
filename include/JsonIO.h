#pragma once
#include <string>

class ExpenseTracker;

namespace JsonIO {
    bool Save(const ExpenseTracker& tracker, const std::string& filepath);
    bool Load(ExpenseTracker& tracker, const std::string& filepath);
}
