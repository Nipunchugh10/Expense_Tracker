#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "Expense_Tracker.h"

// Utility function for case-insensitive string comparison
std::string ToLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

// Constructor
Expense_Tracker::Expense_Tracker() : NextID(1) {
    for (int i = 0; i < 12; i++) {
        MonthlyBudget[i] = 0.0;
    }
}

// Add Expense
void Expense_Tracker::AddExpense(const std::string& description, double amount, int month, const std::string& category) {
    Expense newExpense(NextID++, description, amount, month, category);
    expenses.push_back(newExpense);

    CheckBudgetWarning(month);
}

// Update Expense
void Expense_Tracker::UpdateExpense(int id, const std::string& description, double amount, int month, const std::string& category) {
    Expense* expense = FindExpenseByID(id);
    if (expense) {
        expense->SetDescription(description);
        expense->SetAmount(amount);
        expense->SetMonth(month);
        expense->SetCategory(category);
        std::cout << "Expense updated successfully.\n";
    } else {
        std::cout << "Expense with ID " << id << " not found.\n";
    }
}

// Delete Expense
bool Expense_Tracker::DeleteExpense(int id) {
    for (auto it = expenses.begin(); it != expenses.end(); ++it) {
        if (it->GetID() == id) {
            expenses.erase(it);
            std::cout << "Expense deleted successfully.\n";
            return true;
        }
    }
    std::cout << "Expense with ID " << id << " not found.\n";
    return false;
}

// View All Expenses
void Expense_Tracker::ViewAllExpenses() const {
    if (expenses.empty()) {
        std::cout << "No expenses to display.\n";
        return;
    }

    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Description"
              << std::setw(10) << "Amount"
              << std::setw(15) << "Month"
              << std::setw(15) << "Category" << std::endl;

    for (const auto& exp : expenses) {
        std::cout << std::left << std::setw(5) << exp.GetID()
                  << std::setw(20) << exp.GetDescription()
                  << std::setw(10) << exp.GetAmount()
                  << std::setw(15) << exp.GetMonth()
                  << std::setw(15) << exp.GetCategory() << std::endl;
    }
}

// View Expenses by Month
void Expense_Tracker::ViewExpensesByMonth(int month) const {
    bool found = false;
    for (const auto& exp : expenses) {
        if (exp.GetMonth() == month) {
            if (!found) {
                std::cout << "Expenses for month " << month << ":\n";
                found = true;
            }
            std::cout << "ID: " << exp.GetID() << ", "
                      << "Description: " << exp.GetDescription() << ", "
                      << "Amount: $" << exp.GetAmount() << ", "
                      << "Category: " << exp.GetCategory() << "\n";
        }
    }
    if (!found) {
        std::cout << "No expenses found for month " << month << ".\n";
    }
}

// Expense Summary
void Expense_Tracker::ViewSummary() const {
    double total = 0.0;
    for (const auto& exp : expenses) {
        total += exp.GetAmount();
    }
    std::cout << "Total expenses so far: $" << total << "\n";
}

// Filter by Category
void Expense_Tracker::FilterByCategory(const std::string& category) const {
    bool found = false;
    std::string targetCategory = ToLower(category);

    for (const auto& exp : expenses) {
        if (ToLower(exp.GetCategory()) == targetCategory) {
            if (!found) {
                std::cout << "Expenses in category \"" << category << "\":\n";
                found = true;
            }
            std::cout << "ID: " << exp.GetID()
                      << ", Description: " << exp.GetDescription()
                      << ", Amount: $" << exp.GetAmount()
                      << ", Month: " << exp.GetMonth() << "\n";
        }
    }

    if (!found) {
        std::cout << "No expenses found in category \"" << category << "\".\n";
    }
}

// Set Monthly Budget
void Expense_Tracker::SetMonthlyBudget(int month, double budget) {
    if (month < 1 || month > 12) {
        std::cout << "Invalid month.\n";
        return;
    }
    MonthlyBudget[month - 1] = budget;
    std::cout << "Budget for month " << month << " set to $" << budget << "\n";
}

// Check Budget Warning
void Expense_Tracker::CheckBudgetWarning(int month) const {
    if (month < 1 || month > 12) return;

    double total = 0.0;
    for (const auto& exp : expenses) {
        if (exp.GetMonth() == month) {
            total += exp.GetAmount();
        }
    }
    double budget = MonthlyBudget[month - 1];
    if (budget > 0.0 && total > budget) {
        std::cout << "[WARNING] You have exceeded your budget of $" << budget << " for month " << month << "!\n";
    }
}

// Save Expenses to CSV
void Expense_Tracker::SaveToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file " << filename << " for writing.\n";
        return;
    }

    file << "ID,Description,Amount,Month,Category\n";
    for (const auto& exp : expenses) {
        file << exp.GetID() << "," << exp.GetDescription() << "," << exp.GetAmount() << ","
             << exp.GetMonth() << "," << exp.GetCategory() << "\n";
    }

    file.close();
    std::cout << "Expenses saved to " << filename << "\n";
}

// Load Expenses from CSV
void Expense_Tracker::LoadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file " << filename << " for reading.\n";
        return;
    }

    expenses.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("ID") == 0) continue;  // Skip header row

        std::stringstream ss(line);
        int id, month;
        double amount;
        std::string description, category;

        ss >> id >> description >> amount >> month >> category;
        expenses.emplace_back(id, description, amount, month, category);
        NextID = std::max(NextID, id + 1);
    }

    file.close();
    std::cout << "Expenses loaded from " << filename << "\n";
}

// Main Menu
void DisplayMenu(Expense_Tracker& tracker) {
    int choice;
    do {
        std::cout << "\n--- Expense Tracker Menu ---\n"
                  << "1. Add Expense\n"
                  << "2. Update Expense\n"
                  << "3. Delete Expense\n"
                  << "4. View All Expenses\n"
                  << "5. View Expenses by Month\n"
                  << "6. Filter by Category\n"
                  << "7. View Summary\n"
                  << "8. Set Monthly Budget\n"
                  << "9. Save to CSV\n"
                  << "10. Load from CSV\n"
                  << "11. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string desc, category;
                double amount;
                int month;
                std::cout << "Enter description: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                std::cout << "Enter amount: ";
                std::cin >> amount;
                std::cout << "Enter month (1-12): ";
                std::cin >> month;
                std::cout << "Enter category: ";
                std::cin.ignore();
                std::getline(std::cin, category);
                tracker.AddExpense(desc, amount, month, category);
                break;
            }
            case 2: {
                int id;
                std::string desc, category;
                double amount;
                int month;
                std::cout << "Enter ID to update: ";
                std::cin >> id;
                std::cout << "Enter new description: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                std::cout << "Enter new amount: ";
                std::cin >> amount;
                std::cout << "Enter new month (1-12): ";
                std::cin >> month;
                std::cout << "Enter new category: ";
                std::cin.ignore();
                std::getline(std::cin, category);
                tracker.UpdateExpense(id, desc, amount, month, category);
                break;
            }
            case 3: {
                int id;
                std::cout << "Enter ID to delete: ";
                std::cin >> id;
                tracker.DeleteExpense(id);
                break;
            }
            case 4:
                tracker.ViewAllExpenses();
                break;
            case 5: {
                int month;
                std::cout << "Enter month (1-12): ";
                std::cin >> month;
                tracker.ViewExpensesByMonth(month);
                break;
            }
            case 6: {
                std::string category;
                std::cout << "Enter category: ";
                std::cin.ignore();
                std::getline(std::cin, category);
                tracker.FilterByCategory(category);
                break;
            }
            case 7:
                tracker.ViewSummary();
                break;
            case 8: {
                int month;
                double budget;
                std::cout << "Enter month (1-12): ";
                std::cin >> month;
                std::cout << "Enter budget: ";
                std::cin >> budget;
                tracker.SetMonthlyBudget(month, budget);
                break;
            }
            case 9: {
                std::string filename;
                std::cout << "Enter filename to save: ";
                std::cin >> filename;
                tracker.SaveToCSV(filename);
                break;
            }
            case 10: {
                std::string filename;
                std::cout << "Enter filename to load: ";
                std::cin >> filename;
                tracker.LoadFromCSV(filename);
                break;
            }
            case 11:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 11);
}

// Main Function
int main() {
    Expense_Tracker tracker;
    DisplayMenu(tracker);
    return 0;
}


/*

Key Steps in the DeleteExpense() Function
Iterator Loop:

The function uses an iterator (it) to traverse the list expenses.
expenses.begin() points to the first element of the list,
and expenses.end() represents the position after the last element.
++it moves the iterator to the next element in the list.

Condition Check:

For each element in the list,
the function calls GetID() to fetch the unique ID of the expense.
If the current expense's ID matches the id parameter passed to the function,
it proceeds to delete the expense.

Delete the Expense:

The erase() method of the std::list
(or similar container) is used to remove the expense at the current iterator position (it).
This is efficient because erase() adjusts the iterators automatically.

Output and Return:

If a match is found and the expense is deleted,
the function prints "Expense Deleted." and returns true.

Handle Not Found:

If the loop completes without finding a matching ID,
the function prints "Expense with ID No. : <id> Not Found." and returns false.

*/