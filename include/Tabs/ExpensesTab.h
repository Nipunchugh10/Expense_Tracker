#pragma once
#include "FilterCriteria.h"
#include "Expense.h"

class ExpenseTracker;

class ExpensesTab {
public:
    void Render(ExpenseTracker& tracker);

private:
    void RenderFilterBar(ExpenseTracker& tracker);
    void RenderTable(ExpenseTracker& tracker);
    void RenderAddPopup(ExpenseTracker& tracker);
    void RenderEditPopup(ExpenseTracker& tracker);
    void RenderDeleteConfirm(ExpenseTracker& tracker);

    FilterCriteria filter;

    // Add form state
    char   addDesc[256] = {};
    double addAmount = 0.0;
    int    addDate[3] = {2026, 6, 10}; // y, m, d
    char   addCategory[128] = {};
    char   addCurrency[8] = "USD";
    bool   openAddPopup = false;

    // Edit form state
    int    editID = 0;
    char   editDesc[256] = {};
    double editAmount = 0.0;
    int    editDate[3] = {2026, 1, 1};
    char   editCategory[128] = {};
    char   editCurrency[8] = "USD";
    bool   openEditPopup = false;

    // Delete confirmation
    int  deleteID = 0;
    bool openDeleteConfirm = false;

    // Sorting
    int sortCol = -1;
    bool sortAsc = true;

    // Filter bar
    char searchBuf[256] = {};
    int  filterDateFrom[3] = {1900, 1, 1};
    int  filterDateTo[3]   = {2100, 12, 31};
    char filterCategory[128] = {};
    double filterAmountMin = 0.0;
    double filterAmountMax = 1e12;
};
