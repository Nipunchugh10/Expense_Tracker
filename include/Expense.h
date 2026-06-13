#pragma once
#include <string>

struct Date {
    int year  = 2026;
    int month = 1;
    int day   = 1;

    bool operator<(const Date& o) const;
    bool operator<=(const Date& o) const;
    bool operator==(const Date& o) const;
    bool operator>=(const Date& o) const;
    bool operator>(const Date& o) const;

    std::string ToString() const;
    static Date FromString(const std::string& s);
    static Date Today();
    int ToInt() const;
};

class Expense {
public:
    Expense();
    Expense(int id, const std::string& description, double amount,
            const Date& date, const std::string& category,
            const std::string& currency = "USD", int recurringRuleId = 0);

    int         GetID()              const { return id; }
    std::string GetDescription()     const { return description; }
    double      GetAmount()          const { return amount; }
    Date        GetDate()            const { return date; }
    int         GetMonth()           const { return date.month; }
    int         GetYear()            const { return date.year; }
    std::string GetCategory()        const { return category; }
    std::string GetCurrency()        const { return currency; }
    int         GetRecurringRuleId() const { return recurringRuleId; }

    void SetDescription(const std::string& d)  { description = d; }
    void SetAmount(double a)                    { amount = a; }
    void SetDate(const Date& d)                 { date = d; }
    void SetCategory(const std::string& c)      { category = c; }
    void SetCurrency(const std::string& c)      { currency = c; }
    void SetRecurringRuleId(int r)              { recurringRuleId = r; }

private:
    int         id = 0;
    std::string description;
    double      amount = 0.0;
    Date        date;
    std::string category;
    std::string currency = "USD";
    int         recurringRuleId = 0;
};
