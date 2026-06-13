#pragma once
#include <vector>
#include <string>
#include <map>
#include "Expense.h"
#include "RecurringRule.h"
#include "Budget.h"
#include "FilterCriteria.h"

enum class SortField { ID, Date, Description, Amount, Category, Currency };
enum class SortOrder { Ascending, Descending };

class ExpenseTracker {
public:
    ExpenseTracker();

    // Expense CRUD
    void AddExpense(const std::string& desc, double amount, const Date& date,
                    const std::string& category, const std::string& currency = "USD",
                    int recurringRuleId = 0);
    void UpdateExpense(int id, const std::string& desc, double amount,
                       const Date& date, const std::string& category,
                       const std::string& currency);
    bool DeleteExpense(int id);

    // Recurring rule CRUD
    void AddRecurringRule(const RecurringRule& rule);
    void UpdateRecurringRule(const RecurringRule& rule);
    bool DeleteRecurringRule(int id);
    void GenerateRecurringExpenses(const Date& upTo);

    // Queries
    std::vector<const Expense*> GetFilteredExpenses(const FilterCriteria& criteria) const;
    std::vector<const Expense*> GetAllExpenses() const;
    const Expense* FindExpenseByID(int id) const;

    // Analytics
    double GetMonthlyTotal(int year, int month) const;
    double GetYearlyTotal(int year) const;
    std::map<std::string, double> GetCategoryBreakdown(int year, int month = 0) const;
    std::map<int, double> GetMonthlyTotals(int year) const; // month -> total
    std::vector<const Expense*> GetTopExpenses(int year, int count = 10) const;
    std::map<std::string, double> GetCurrencyBreakdown(int year) const;

    // Dropdown data
    std::vector<std::string> GetCategories() const;
    std::vector<std::string> GetCurrencies() const;

    // Sorting
    void SortExpenses(SortField field, SortOrder order);

    // Budget
    BudgetManager& GetBudgetManager() { return budgetManager; }
    const BudgetManager& GetBudgetManager() const { return budgetManager; }

    // Recurring rules access
    std::vector<RecurringRule>& GetRecurringRules() { return recurringRules; }
    const std::vector<RecurringRule>& GetRecurringRules() const { return recurringRules; }

    // Direct access for serialization
    std::vector<Expense>& GetExpenses() { return expenses; }
    const std::vector<Expense>& GetExpensesRef() const { return expenses; }

    int  GetNextExpenseID() const { return nextExpenseID; }
    void SetNextExpenseID(int id) { nextExpenseID = id; }
    int  GetNextRuleID() const { return nextRuleID; }
    void SetNextRuleID(int id) { nextRuleID = id; }

    int GetExpenseCount() const { return static_cast<int>(expenses.size()); }
    bool HasChanges() const { return dirty; }
    void ClearDirty() { dirty = false; }

private:
    std::vector<Expense>       expenses;
    std::vector<RecurringRule>  recurringRules;
    BudgetManager              budgetManager;
    int                        nextExpenseID = 1;
    int                        nextRuleID = 1;
    bool                       dirty = false;
};
