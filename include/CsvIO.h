#pragma once
#include <string>

class ExpenseTracker;

namespace CsvIO {
    bool Export(const ExpenseTracker& tracker, const std::string& filepath);
    bool Import(ExpenseTracker& tracker, const std::string& filepath);
}
