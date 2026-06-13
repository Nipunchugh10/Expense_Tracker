#pragma once

class ExpenseTracker;

class DashboardTab {
public:
    void Render(ExpenseTracker& tracker);

private:
    int selectedYear  = 2026;
    int selectedMonth = 0; // 0 = all months
    bool openResetConfirm = false;
};
