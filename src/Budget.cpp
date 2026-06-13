#include "Budget.h"

void BudgetManager::SetBudget(int year, int month, double amount) {
    budgets[MakeKey(year, month)] = amount;
}

double BudgetManager::GetBudget(int year, int month) const {
    auto it = budgets.find(MakeKey(year, month));
    return (it != budgets.end()) ? it->second : 0.0;
}

bool BudgetManager::HasBudget(int year, int month) const {
    auto it = budgets.find(MakeKey(year, month));
    return it != budgets.end() && it->second > 0.0;
}
