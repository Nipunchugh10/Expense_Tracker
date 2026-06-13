#pragma once
#include <map>

class BudgetManager {
public:
    void   SetBudget(int year, int month, double amount);
    double GetBudget(int year, int month) const;
    bool   HasBudget(int year, int month) const;

    const std::map<int, double>& GetAll() const { return budgets; }
    void Clear() { budgets.clear(); }

    // For serialization
    void SetFromKey(int key, double amount) { budgets[key] = amount; }

private:
    // key = year * 100 + month
    std::map<int, double> budgets;

    static int MakeKey(int year, int month) { return year * 100 + month; }
};
