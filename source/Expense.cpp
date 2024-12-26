#include "Expense.h"

Expense::Expense() 
    : id(0), description(""), amount(0.0), month(0), category("") 
{
}

Expense::Expense(int id, const std::string& description, double amount, int month, const std::string& category)
    : id(id), description(description), amount(amount), month(month), category(category)
{
}

int Expense::GetID() const {
    return id;
}

std::string Expense::GetDescription() const {
    return description;
}

double Expense::GetAmount() const {
    return amount;
}

int Expense::GetMonth() const {
    return month;
}

std::string Expense::GetCategory() const {
    return category;
}

void Expense::SetDescription(const std::string& desc) {
    description = desc;
}

void Expense::SetAmount(double amt) {
    amount = amt;
}

void Expense::SetMonth(int m) {
    month = m;
}

void Expense::SetCategory(const std::string& cat) {
    category = cat;
}
