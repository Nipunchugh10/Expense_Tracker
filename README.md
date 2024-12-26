# EXPENSE TRACKER

A Comprehensive C++ console application for managing personal expenses with features for tracking, categorizing and analyzing spending pattern.

## FEATURES

- **Expense Management**
    - Add expenses with description, amount, month and category.
    - Update existing expense details.
    - Delete expense by ID.
    - View all recorded expenses.
      
- **Analysis & Reporting**
  - Monthly expense summaries.
  - Category-based filtering.
  - Overall spending summary.
  - Monthly budget tracking with warning system.
    
- **Data Persistense**
  - Save Expenses to CSV Files.
  - Load Expenses form saved CSV Files.

## GETTING STARTED

### Prerequisites

  - C++ compiler with C++ 11 support or later.
  - Standard Template Library (STL)

### BUILDING THE PROJECT

1. Clone the Repository
```bash
git https://github.com/Nipunchugh10/Expense_Tracker.git
cd expense-tracker
```
2. Compile the source files
```
g++ main.cpp Expense.cpp Expense_Tracker.cpp -o expense_tracker
```
3.Run The Application
```
./expense_tracker
```

## USAGE

The Application provides an interactive menu-driven interface with the following options:

1. Add Expense - Record a new expense.
2. Update Expense - Modify an existing expense.
3. Delete Expesne - Remove an expense by ID.
4. View All Expenses - Display all recorded expenses.
5. View Expenses by Month - Filter expenses for a specific month.
6. View Summary - Show overall spending statistics.
7. View Monthly Summary - Display Statistics for a specific month.
8. Filter by Category - Show expenses in a specific category.
9. Set Monthly Budget - Configure budget limits.
10. Save to CSV - Export expense to a file.
11. Load from CSV - Import expenses form a file.

### xample Usage

```cpp
// Adding a new expense
Enter Description: Grocery Shopping
Enter Amount: 150.50
Enter Month (1-12): 3
Enter Category: Food

// Setting a monthly budget
Enter Month (1-12): 3
Enter Budget: 1000.00
```

## Project Structure

- `main.cpp` - Program entry point and menu interface
- `Expense.h/cpp` - Expense class definition and implementation
- `Expense_Tracker.h/cpp` - Core expense management functionality

## Features in Detail

### Budget Management
- Set monthly budgets
- Automatic warnings when expenses exceed budget
- Track spending patterns across different months

### Data Validation
- Input validation for months (1-12)
- Proper error handling for file operations
- Protection against invalid user inputs

### Expense Categorization
- Flexible category system
- Category-based filtering and analysis
- Case-insensitive category matching

## Contributing

Contributions are welcome! Feel free to submit issues and pull requests.

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgments

- Inspired by the need for simple, efficient expense tracking
- Built with modern C++ practices
- Designed for easy extension and modification

## Contact

For questions or feedback, please open an issue in the GitHub repository.
