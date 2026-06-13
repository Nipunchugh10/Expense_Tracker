#pragma once
#include <string>
#include <vector>
#include "Expense.h"

enum class Frequency { Daily, Weekly, Monthly, Yearly };

class RecurringRule {
public:
    RecurringRule();
    RecurringRule(int id, const std::string& description, double amount,
                 const std::string& category, const std::string& currency,
                 Frequency freq, const Date& start, const Date& end, bool active = true);

    int         GetID()          const { return id; }
    std::string GetDescription() const { return description; }
    double      GetAmount()      const { return amount; }
    std::string GetCategory()    const { return category; }
    std::string GetCurrency()    const { return currency; }
    Frequency   GetFrequency()   const { return frequency; }
    Date        GetStartDate()   const { return startDate; }
    Date        GetEndDate()     const { return endDate; }
    bool        IsActive()       const { return active; }

    void SetDescription(const std::string& d) { description = d; }
    void SetAmount(double a)                  { amount = a; }
    void SetCategory(const std::string& c)    { category = c; }
    void SetCurrency(const std::string& c)    { currency = c; }
    void SetFrequency(Frequency f)            { frequency = f; }
    void SetStartDate(const Date& d)          { startDate = d; }
    void SetEndDate(const Date& d)            { endDate = d; }
    void SetActive(bool a)                    { active = a; }

    std::vector<Date> GetOccurrences(const Date& rangeStart, const Date& rangeEnd) const;

    static std::string FrequencyToString(Frequency f);
    static Frequency   FrequencyFromString(const std::string& s);

private:
    int         id = 0;
    std::string description;
    double      amount = 0.0;
    std::string category;
    std::string currency = "USD";
    Frequency   frequency = Frequency::Monthly;
    Date        startDate;
    Date        endDate = {2100, 12, 31};
    bool        active = true;
};
