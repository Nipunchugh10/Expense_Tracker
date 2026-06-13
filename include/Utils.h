#pragma once
#include <string>
#include <vector>
#include "Expense.h"

namespace Utils {
    std::string ToLower(const std::string& s);
    bool ContainsCI(const std::string& haystack, const std::string& needle);
    std::string Trim(const std::string& s);

    // CSV helpers
    std::string EscapeCSVField(const std::string& field);
    std::string ParseCSVField(const std::string& line, size_t& pos);
    bool IsValidFilename(const std::string& filename);

    // Date helpers
    int DaysInMonth(int year, int month);
    bool IsLeapYear(int year);
    Date AddMonths(const Date& d, int months);
    Date AddDays(const Date& d, int days);
    Date AddYears(const Date& d, int years);
}
