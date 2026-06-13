#pragma once
#include "ExpenseTracker.h"
#include "Tabs/DashboardTab.h"
#include "Tabs/ExpensesTab.h"
#include "Tabs/BudgetTab.h"
#include "Tabs/AnalyticsTab.h"
#include <string>

class App {
public:
    App();

    void Init();
    void Render();
    void Shutdown();

private:
    void RenderMenuBar();
    void RenderStatusBar();
    void AutoSave();

    ExpenseTracker tracker;
    DashboardTab   dashboardTab;
    ExpensesTab    expensesTab;
    BudgetTab      budgetTab;
    AnalyticsTab   analyticsTab;

    std::string dataPath = "data/expenses.json";
    float       autoSaveTimer = 0.0f;
    float       autoSaveInterval = 60.0f;
    std::string statusMessage;
    float       statusTimer = 0.0f;
};
