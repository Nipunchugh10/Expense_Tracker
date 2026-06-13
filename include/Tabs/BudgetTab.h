#pragma once

class ExpenseTracker;

class BudgetTab {
public:
    void Render(ExpenseTracker& tracker);

private:
    int selectedYear = 2026;
    double budgetInputs[12] = {}; // index 0 = Jan, 11 = Dec
    bool   initialized = false;

    void LoadBudgets(ExpenseTracker& tracker);
};
