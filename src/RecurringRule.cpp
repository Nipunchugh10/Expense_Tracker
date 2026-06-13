#include "RecurringRule.h"
#include "Utils.h"

RecurringRule::RecurringRule() = default;

RecurringRule::RecurringRule(int id, const std::string& description, double amount,
                             const std::string& category, const std::string& currency,
                             Frequency freq, const Date& start, const Date& end, bool active)
    : id(id), description(description), amount(amount), category(category),
      currency(currency), frequency(freq), startDate(start), endDate(end), active(active)
{
}

std::vector<Date> RecurringRule::GetOccurrences(const Date& rangeStart, const Date& rangeEnd) const {
    std::vector<Date> dates;
    if (!active) return dates;

    Date current = startDate;
    // Advance to rangeStart if startDate is before it
    while (current < rangeStart && current <= endDate) {
        switch (frequency) {
            case Frequency::Daily:   current = Utils::AddDays(current, 1); break;
            case Frequency::Weekly:  current = Utils::AddDays(current, 7); break;
            case Frequency::Monthly: current = Utils::AddMonths(current, 1); break;
            case Frequency::Yearly:  current = Utils::AddYears(current, 1); break;
        }
    }

    while (current <= rangeEnd && current <= endDate) {
        if (current >= rangeStart) {
            dates.push_back(current);
        }
        switch (frequency) {
            case Frequency::Daily:   current = Utils::AddDays(current, 1); break;
            case Frequency::Weekly:  current = Utils::AddDays(current, 7); break;
            case Frequency::Monthly: current = Utils::AddMonths(current, 1); break;
            case Frequency::Yearly:  current = Utils::AddYears(current, 1); break;
        }
        if (dates.size() > 10000) break; // safety limit
    }

    return dates;
}

std::string RecurringRule::FrequencyToString(Frequency f) {
    switch (f) {
        case Frequency::Daily:   return "Daily";
        case Frequency::Weekly:  return "Weekly";
        case Frequency::Monthly: return "Monthly";
        case Frequency::Yearly:  return "Yearly";
    }
    return "Monthly";
}

Frequency RecurringRule::FrequencyFromString(const std::string& s) {
    if (s == "Daily")   return Frequency::Daily;
    if (s == "Weekly")  return Frequency::Weekly;
    if (s == "Yearly")  return Frequency::Yearly;
    return Frequency::Monthly;
}
