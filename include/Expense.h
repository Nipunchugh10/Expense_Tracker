#ifndef EXPENSE_H
#define EXPENSE_H

#include<string>

class Expense{
private:
    int id;
    std::string description;
    double amount;
    int month;
    std::string category;
public:
    // CONSTRUCTORS
    Expense();
    Expense(int id, const std::string& description, double amount, int month, const std::string& category);

    // GETTERS
    int GetID() const;
    std::string GetDescription() const;
    double GetAmount() const;
    int GetMonth() const;
    std::string GetCategory() const;

    // SETTERS
    void SetDescription(const std::string& desc);
    void SetAmount(double amt);
    void SetMonth(int m);
    void SetCategory(const std::string& cat);
};

#endif
