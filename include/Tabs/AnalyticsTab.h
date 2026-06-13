#pragma once

class ExpenseTracker;

class AnalyticsTab {
public:
    void Render(ExpenseTracker& tracker);

private:
    int selectedYear = 2026;
};
