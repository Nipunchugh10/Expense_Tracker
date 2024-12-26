#include <iostream>
#include <string>
#include <limits>
#include "Expense_Tracker.h"

void DisplayMenu() {
    std::cout << "\nExpense Tracker Menu:\n"
              << "1. Add Expense\n"
              << "2. Update Expense\n"
              << "3. Delete Expense\n"
              << "4. View All Expenses\n"
              << "5. View Expenses by Month\n"
              << "6. View Summary\n"
              << "7. View Monthly Summary\n"
              << "8. Filter by Category\n"
              << "9. Set Monthly Budget\n"
              << "10. Save To CSV\n"
              << "11. Load from CSV\n"
              << "0. Exit\n"
              << "Enter your Choice: ";
}

int main() {
    Expense_Tracker tracker;

    while (true) {
        DisplayMenu();

        int choice;
        std::cin >> choice;

        // Validate choice input
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear remaining input

        if (choice == 0) {
            std::cout << "Exiting...\n";
            break;
        }

        int id, month;
        double amount;
        std::string description, category, filename;

        switch (choice) {
            case 1:
                std::cout << "Enter Description: ";
                std::getline(std::cin, description);
                std::cout << "Enter Amount: ";
                std::cin >> amount;
                std::cout << "Enter Month (1-12): ";
                std::cin >> month;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter Category: ";
                std::getline(std::cin, category);
                tracker.AddExpense(description, amount, month, category);
                break;

            case 2:
                std::cout << "Enter Expense ID to Update: ";
                std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter New Description: ";
                std::getline(std::cin, description);
                std::cout << "Enter New Amount: ";
                std::cin >> amount;
                std::cout << "Enter New Month (1-12): ";
                std::cin >> month;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter New Category: ";
                std::getline(std::cin, category);
                tracker.UpdateExpense(id, description, amount, month, category);
                break;

            case 3:
                std::cout << "Enter Expense ID to Delete: ";
                std::cin >> id;
                tracker.DeleteExpense(id);
                break;

            case 4:
                tracker.ViewAllExpenses();
                break;

            case 5:
                std::cout << "Enter Month (1-12): ";
                std::cin >> month;
                tracker.ViewExpensesByMonth(month);
                break;

            case 6:
                tracker.ViewSummary();
                break;

            case 7:
                std::cout << "Enter Month (1-12): ";
                std::cin >> month;
                tracker.ViewMonthlySummary(month);
                break;

            case 8:
                std::cout << "Enter Category: ";
                std::getline(std::cin, category);
                tracker.FilterByCategory(category);
                break;

            case 9:
                std::cout << "Enter Month (1-12): ";
                std::cin >> month;
                std::cout << "Enter Budget: ";
                std::cin >> amount;
                tracker.SetMonthlyBudget(month, amount);
                break;

            case 10:
                std::cout << "Enter Filename to Save: ";
                std::cin >> filename;
                tracker.SaveToCSV(filename);
                break;

            case 11:
                std::cout << "Enter Filename to Load: ";
                std::cin >> filename;
                tracker.LoadFromCSV(filename);
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
