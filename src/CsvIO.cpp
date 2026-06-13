#include "CsvIO.h"
#include "ExpenseTracker.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

namespace CsvIO {

bool Export(const ExpenseTracker& tracker, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << "ID,Date,Description,Amount,Currency,Category\n";
    for (auto& e : tracker.GetExpensesRef()) {
        file << e.GetID() << ","
             << e.GetDate().ToString() << ","
             << Utils::EscapeCSVField(e.GetDescription()) << ","
             << e.GetAmount() << ","
             << Utils::EscapeCSVField(e.GetCurrency()) << ","
             << Utils::EscapeCSVField(e.GetCategory()) << "\n";
    }

    file.close();
    return true;
}

bool Import(ExpenseTracker& tracker, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    bool firstLine = true;
    bool oldFormat = false;
    int loadedCount = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (firstLine) {
            firstLine = false;
            // Detect format
            std::string headerLower = Utils::ToLower(line);
            if (headerLower.find("id") != std::string::npos) {
                // Check if it's old format (ID,Description,Amount,Month,Category)
                // or new format (ID,Date,Description,Amount,Currency,Category)
                oldFormat = (headerLower.find("month") != std::string::npos);
                continue; // skip header
            }
        }

        size_t pos = 0;
        try {
            if (oldFormat) {
                // Old format: ID,Description,Amount,Month,Category
                std::string idStr    = Utils::ParseCSVField(line, pos);
                std::string desc     = Utils::ParseCSVField(line, pos);
                std::string amtStr   = Utils::ParseCSVField(line, pos);
                std::string monthStr = Utils::ParseCSVField(line, pos);
                std::string category = Utils::ParseCSVField(line, pos);

                int id       = std::stoi(idStr);
                double amt   = std::stod(amtStr);
                int month    = std::stoi(monthStr);

                if (month < 1 || month > 12 || amt < 0) continue;

                Date d;
                d.year = 2026;
                d.month = month;
                d.day = 1;

                tracker.GetExpenses().emplace_back(id, desc, amt, d, category, "USD", 0);
                int nextID = tracker.GetNextExpenseID();
                if (id >= nextID) tracker.SetNextExpenseID(id + 1);
                loadedCount++;
            } else {
                // New format: ID,Date,Description,Amount,Currency,Category
                std::string idStr    = Utils::ParseCSVField(line, pos);
                std::string dateStr  = Utils::ParseCSVField(line, pos);
                std::string desc     = Utils::ParseCSVField(line, pos);
                std::string amtStr   = Utils::ParseCSVField(line, pos);
                std::string currency = Utils::ParseCSVField(line, pos);
                std::string category = Utils::ParseCSVField(line, pos);

                int id     = std::stoi(idStr);
                double amt = std::stod(amtStr);
                Date d     = Date::FromString(dateStr);

                if (amt < 0) continue;
                if (currency.empty()) currency = "USD";

                tracker.GetExpenses().emplace_back(id, desc, amt, d, category, currency, 0);
                int nextID = tracker.GetNextExpenseID();
                if (id >= nextID) tracker.SetNextExpenseID(id + 1);
                loadedCount++;
            }
        } catch (...) {
            continue; // skip malformed lines
        }
    }

    file.close();
    return loadedCount > 0;
}

} // namespace CsvIO
