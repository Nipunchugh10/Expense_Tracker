#include "Expense.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// ── Date ──────────────────────────────────────────────────────────

int Date::ToInt() const { return year * 10000 + month * 100 + day; }

bool Date::operator<(const Date& o)  const { return ToInt() < o.ToInt(); }
bool Date::operator<=(const Date& o) const { return ToInt() <= o.ToInt(); }
bool Date::operator==(const Date& o) const { return ToInt() == o.ToInt(); }
bool Date::operator>=(const Date& o) const { return ToInt() >= o.ToInt(); }
bool Date::operator>(const Date& o)  const { return ToInt() > o.ToInt(); }

std::string Date::ToString() const {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(4) << year << '-'
       << std::setw(2) << month << '-' << std::setw(2) << day;
    return ss.str();
}

Date Date::FromString(const std::string& s) {
    Date d;
    if (s.size() >= 10 && s[4] == '-' && s[7] == '-') {
        d.year  = std::stoi(s.substr(0, 4));
        d.month = std::stoi(s.substr(5, 2));
        d.day   = std::stoi(s.substr(8, 2));
    }
    return d;
}

Date Date::Today() {
    Date d;
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    if (tm) {
        d.year  = tm->tm_year + 1900;
        d.month = tm->tm_mon + 1;
        d.day   = tm->tm_mday;
    }
    return d;
}

// ── Expense ───────────────────────────────────────────────────────

Expense::Expense() = default;

Expense::Expense(int id, const std::string& description, double amount,
                 const Date& date, const std::string& category,
                 const std::string& currency, int recurringRuleId)
    : id(id), description(description), amount(amount), date(date),
      category(category), currency(currency), recurringRuleId(recurringRuleId)
{
}
