#ifndef Expense_Tracker_H
#define Expense_Tracker_H

#include<vector>
#include<string>
#include"Expense.h"

class Expense_Tracker {

private:
    std::vector<Expense> expenses;
    int NextID; // Assigning unique IDs
    double MonthlyBudget[12]; // 1-12 for each month, store budget for each month

public:
    // CONSTRUCTOR
    Expense_Tracker();


    //OPERATIONAL FUNCTIONALITIES
    void AddExpense(const std::string& description, double amount, int month, const std::string& category);
    void UpdateExpense(int id, const std::string& description, double amount, int month, const std::string& category);
    bool DeleteExpense(int id);


    //VIEW FUNCTIONALITIES
    void ViewAllExpenses() const;
    void ViewExpensesByMonth(int month) const;
    void ViewSummary() const;
    void ViewMonthlySummary(int month) const;


    //ADDITIONAL FUNCTIONALITIES
    void FilterByCategory(const std::string& category) const;
    void SetMonthlyBudget(int month, double budget);
    void CheckBudgetWarning(int month) const;


    // FILE I/O
    void SaveToCSV(const std::string& filename) const;
    void LoadFromCSV(const std::string& filename);

private:
    Expense* FindExpenseByID(int id);
};  

#endif