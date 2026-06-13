# Expense Tracker Plus
An interactive, cross-platform C++ desktop application for personal finance management, budgeting, and interactive data analytics. Built on top of **Dear ImGui**, **OpenGL 3**, and **ImPlot**, this project provides a hardware-accelerated, responsive, and visually appealing native user interface to track expenses, manage recurring payments, and analyze spending habits.

---

## Key Features
- **Comprehensive Expense CRUD**: Log, edit, and delete transactions with details including description, amount, date, category, and currency (multi-currency support).
- **Interactive Data Analytics (ImPlot)**:
  - Monthly spending trends.
  - Category breakdown via donut/pie graphs.
  - Distribution of expenditures by currency.
  - Top 10 high-value transactions list.
- **Budgeting System**: Establish and manage monthly financial limits to track target vs. actual spending.
- **Smart Recurring Transactions**:
  - Automatically generate recurring transactions based on customizable frequency criteria (Daily, Weekly, Monthly, Yearly).
  - Handles scheduling ranges (start date to end date).
- **Data Portability (Import/Export)**:
  - Local persistence via JSON serialization (auto-saves changes periodically and on shutdown).
  - Export and Import transactions to/from CSV (fully compatible with Microsoft Excel, Google Sheets, etc.).
- **Responsive Native Desktop UI**: Fully interactive multi-tab interface (Dashboard, Expenses, Budget, Analytics) with customizable text scaling and keyboard navigation.

---

## Project Directory Structure
The repository follows a clean, modular object-oriented project design separating the Core Logic, User Interface, and Serialization layers.

```text
Expense_Tracker/
├── CMakeLists.txt         # Main build configuration script (C++17)
├── build.bat              # Simple Windows compile script (Legacy compile fallback)
├── data/                  # Local persistence directory
│   └── expenses.json      # Auto-saved JSON ledger database
├── include/               # Header Files (.h)
│   ├── App.h              # Application main loop & Window Orchestrator
│   ├── Budget.h           # Budget tracker module
│   ├── CsvIO.h            # CSV data parser & serializer
│   ├── Expense.h          # Data structures for Expenses & Dates
│   ├── ExpenseTracker.h   # Core tracker backend & state manager
│   ├── FilterCriteria.h   # Expense sorting and filter structure
│   ├── JsonIO.h           # JSON parser & serialization backend
│   ├── RecurringRule.h    # Recurring transaction scheduler logic
│   ├── Utils.h            # Date formatting and helper functions
│   └── Tabs/              # ImGui GUI Tab views
│       ├── AnalyticsTab.h
│       ├── BudgetTab.h
│       ├── DashboardTab.h
│       └── ExpensesTab.h
├── src/                   # Source Files (.cpp)
│   ├── App.cpp
│   ├── Budget.cpp
│   ├── CsvIO.cpp
│   ├── Expense.cpp
│   ├── ExpenseTracker.cpp
│   ├── FilterCriteria.cpp
│   ├── JsonIO.cpp
│   ├── RecurringRule.cpp
│   ├── Utils.cpp
│   ├── main.cpp           # App Entrypoint (OpenGL Context, ImGui, ImPlot setup)
│   └── Tabs/              # ImGui UI Implementation
│       ├── AnalyticsTab.cpp
│       ├── BudgetTab.cpp
│       ├── DashboardTab.cpp
│       └── ExpensesTab.cpp
└── third_party/           # Vendor Dependencies (GLFW, Dear ImGui, ImPlot, nlohmann/json)
```

---

## Tech Stack & Dependencies
* **Language**: C++17
* **Build System**: CMake 3.16+
* **GUI Toolkit**: [Dear ImGui](https://github.com/ocornut/imgui) (v1.89+ with Docking/Viewport features)
* **Plotting Engine**: [ImPlot](https://github.com/epezent/implot) (Hardware-accelerated charting library)
* **Window & Input**: GLFW 3
* **Graphics API**: OpenGL 3.3 Core Profile
* **JSON Library**: [nlohmann/json](https://github.com/nlohmann/json) (Modern JSON for C++)

---

## Getting Started & Compilation

### Prerequisites
Make sure you have a modern C++ compiler supporting C++17 (e.g., GCC/MinGW, Clang, or MSVC) and CMake installed on your path.

#### Windows (using MinGW Makefiles)
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Nipunchugh10/Expense_Tracker.git
   cd Expense_Tracker
   ```
2. **Configure with CMake**:
   ```bash
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   ```
3. **Build the Project**:
   ```bash
   cmake --build . --config Release
   ```
4. **Run the Executable**:
   ```bash
   cd bin
   ./Expense_Tracker.exe
   ```

#### Linux / macOS
1. Install GLFW3 dependency via package manager:
   - *Ubuntu/Debian*: `sudo apt-get install libglfw3-dev`
   - *macOS (Homebrew)*: `brew install glfw`
2. Configure, build, and run the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ./bin/Expense_Tracker
   ```

---

## Architecture & System Design

### 1. Data Model (`Expense` & `RecurringRule`)
- `Expense` stores individual transaction records with attributes (`description`, `amount`, `category`, `currency`, `date`).
- `RecurringRule` defines how often a transaction repeats. When the application loads, `ExpenseTracker::GenerateRecurringExpenses` evaluates active rules against the current system date and automatically fills in occurrences.

### 2. State Controller (`ExpenseTracker`)
Acts as a central manager (Model-View-Controller design pattern). It exposes APIs for creating, reading, updating, sorting, filtering, and performing high-performance analytics query aggregations. It maintains an internal *dirty* flag indicating unsaved state modifications.

### 3. File I/O Engine (`JsonIO` & `CsvIO`)
- **JSON Serialization**: Integrates with `nlohmann_json` to marshal/unmarshal arrays of expenses, budgets, and rules.
- **CSV Export/Import**: Generates RFC 4180-compliant comma-separated values to support seamless data migration.

### 4. UI Layer (`App` & `Tabs`)
The UI is divided into 4 core tab components:
- **Dashboard**: High-level financial status widget, recent transactions, auto-save status indicators, and budget progression bars.
- **Expenses**: Comprehensive searchable table layout. Supports filter criteria query-by-category, inline search, and multi-column sorting.
- **Budget**: Set monthly targets, select specific months, and review real-time budget utilization cards.
- **Analytics**: Beautiful dashboard containing multiple ImPlot widgets representing visual metrics.

---

## Visual Layout Preview
The interface is designed with a sleek dark theme style:
- **Menu Bar**: Top-aligned file management controls (Save, Import/Export CSV, Export JSON).
- **Dashboard Widgets**: Clean summaries of total expenses, active budget tracking, and list of transactions.
- **Dynamic Charts**: Interactive ImPlot charts featuring tooltip hovers, panning, zooming, and legend toggles for financial analysis.
- **Status Bar**: Live status tracking indicating unsaved modifications or auto-save intervals.

---

## Roadmap / Future Scope
* **Smart Categorization**: Integrate basic rule-based or machine learning predictions to auto-categorize imported transactions.
* **Database Backend**: Migrate from static JSON file persistence to a lightweight SQLite database for scaling to thousands of records.
* **Dark/Light Mode Toggle**: Allow users to dynamically transition between modern themes.
* **Goal Tracking**: Create sub-accounts or target savings buckets (e.g., saving for travel, electronics).

---

## License
This project is open-source and available under the [MIT License](LICENSE).

---

## Acknowledgments
* **Dear ImGui** by Omar Cornut for the exceptional GUI framework.
* **ImPlot** by Evan Pezent for interactive C++ plotting capabilities.
* **nlohmann/json** by Niels Lohmann for seamless modern C++ JSON manipulation.
