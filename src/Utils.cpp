#include "Utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace Utils {

std::string ToLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return r;
}

bool ContainsCI(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    return ToLower(haystack).find(ToLower(needle)) != std::string::npos;
}

std::string Trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string EscapeCSVField(const std::string& field) {
    bool needsQuoting = false;
    for (char c : field) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needsQuoting = true;
            break;
        }
    }
    // CSV injection protection
    if (!field.empty() && (field[0] == '=' || field[0] == '+' ||
                           field[0] == '-' || field[0] == '@')) {
        needsQuoting = true;
    }
    if (!needsQuoting) return field;

    std::string escaped = "\"";
    for (char c : field) {
        if (c == '"') escaped += "\"\"";
        else escaped += c;
    }
    escaped += '"';
    return escaped;
}

std::string ParseCSVField(const std::string& line, size_t& pos) {
    if (pos >= line.size()) return "";

    std::string field;
    if (line[pos] == '"') {
        pos++; // skip opening quote
        while (pos < line.size()) {
            if (line[pos] == '"') {
                if (pos + 1 < line.size() && line[pos + 1] == '"') {
                    field += '"';
                    pos += 2;
                } else {
                    pos++; // skip closing quote
                    break;
                }
            } else {
                field += line[pos++];
            }
        }
        if (pos < line.size() && line[pos] == ',') pos++;
    } else {
        while (pos < line.size() && line[pos] != ',') {
            field += line[pos++];
        }
        if (pos < line.size()) pos++; // skip comma
    }
    return field;
}

bool IsValidFilename(const std::string& filename) {
    if (filename.empty()) return false;
    if (filename[0] == '/' || filename[0] == '\\') return false;
    if (filename.find(':') != std::string::npos) return false;
    if (filename.find("..") != std::string::npos) return false;
    return true;
}

bool IsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DaysInMonth(int year, int month) {
    static const int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && IsLeapYear(year)) return 29;
    if (month >= 1 && month <= 12) return days[month];
    return 30;
}

Date AddDays(const Date& d, int days) {
    Date r = d;
    r.day += days;
    while (r.day > DaysInMonth(r.year, r.month)) {
        r.day -= DaysInMonth(r.year, r.month);
        r.month++;
        if (r.month > 12) { r.month = 1; r.year++; }
    }
    while (r.day < 1) {
        r.month--;
        if (r.month < 1) { r.month = 12; r.year--; }
        r.day += DaysInMonth(r.year, r.month);
    }
    return r;
}

Date AddMonths(const Date& d, int months) {
    Date r = d;
    r.month += months;
    while (r.month > 12) { r.month -= 12; r.year++; }
    while (r.month < 1)  { r.month += 12; r.year--; }
    int maxDay = DaysInMonth(r.year, r.month);
    if (r.day > maxDay) r.day = maxDay;
    return r;
}

Date AddYears(const Date& d, int years) {
    Date r = d;
    r.year += years;
    int maxDay = DaysInMonth(r.year, r.month);
    if (r.day > maxDay) r.day = maxDay;
    return r;
}

} // namespace Utils
