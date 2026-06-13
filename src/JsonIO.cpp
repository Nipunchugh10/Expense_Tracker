#include "JsonIO.h"
#include "ExpenseTracker.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace JsonIO {

bool Save(const ExpenseTracker& tracker, const std::string& filepath) {
    // Ensure directory exists
    fs::path p(filepath);
    if (p.has_parent_path()) {
        fs::create_directories(p.parent_path());
    }

    json j;

    // Expenses
    json expArr = json::array();
    for (auto& e : tracker.GetExpensesRef()) {
        json ej;
        ej["id"]          = e.GetID();
        ej["description"] = e.GetDescription();
        ej["amount"]      = e.GetAmount();
        ej["date"]        = e.GetDate().ToString();
        ej["category"]    = e.GetCategory();
        ej["currency"]    = e.GetCurrency();
        ej["recurringRuleId"] = e.GetRecurringRuleId();
        expArr.push_back(ej);
    }
    j["expenses"] = expArr;
    j["nextExpenseID"] = tracker.GetNextExpenseID();

    // Recurring rules
    json ruleArr = json::array();
    for (auto& r : tracker.GetRecurringRules()) {
        json rj;
        rj["id"]          = r.GetID();
        rj["description"] = r.GetDescription();
        rj["amount"]      = r.GetAmount();
        rj["category"]    = r.GetCategory();
        rj["currency"]    = r.GetCurrency();
        rj["frequency"]   = RecurringRule::FrequencyToString(r.GetFrequency());
        rj["startDate"]   = r.GetStartDate().ToString();
        rj["endDate"]     = r.GetEndDate().ToString();
        rj["active"]      = r.IsActive();
        ruleArr.push_back(rj);
    }
    j["recurringRules"] = ruleArr;
    j["nextRuleID"] = tracker.GetNextRuleID();

    // Budgets
    json budgetObj = json::object();
    for (auto& [key, val] : tracker.GetBudgetManager().GetAll()) {
        budgetObj[std::to_string(key)] = val;
    }
    j["budgets"] = budgetObj;

    std::ofstream file(filepath);
    if (!file.is_open()) return false;
    file << j.dump(2);
    file.close();
    return true;
}

bool Load(ExpenseTracker& tracker, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    json j;
    try {
        file >> j;
    } catch (...) {
        return false;
    }
    file.close();

    // Clear existing data
    tracker.GetExpenses().clear();
    tracker.GetRecurringRules().clear();
    tracker.GetBudgetManager().Clear();

    // Load expenses
    if (j.contains("expenses") && j["expenses"].is_array()) {
        for (auto& ej : j["expenses"]) {
            Date d = Date::FromString(ej.value("date", "2026-01-01"));
            Expense e(
                ej.value("id", 0),
                ej.value("description", ""),
                ej.value("amount", 0.0),
                d,
                ej.value("category", ""),
                ej.value("currency", "USD"),
                ej.value("recurringRuleId", 0)
            );
            tracker.GetExpenses().push_back(e);
        }
    }
    tracker.SetNextExpenseID(j.value("nextExpenseID", 1));

    // Load recurring rules
    if (j.contains("recurringRules") && j["recurringRules"].is_array()) {
        for (auto& rj : j["recurringRules"]) {
            RecurringRule r(
                rj.value("id", 0),
                rj.value("description", ""),
                rj.value("amount", 0.0),
                rj.value("category", ""),
                rj.value("currency", "USD"),
                RecurringRule::FrequencyFromString(rj.value("frequency", "Monthly")),
                Date::FromString(rj.value("startDate", "2026-01-01")),
                Date::FromString(rj.value("endDate", "2100-12-31")),
                rj.value("active", true)
            );
            tracker.GetRecurringRules().push_back(r);
        }
    }
    tracker.SetNextRuleID(j.value("nextRuleID", 1));

    // Load budgets
    if (j.contains("budgets") && j["budgets"].is_object()) {
        for (auto& [key, val] : j["budgets"].items()) {
            tracker.GetBudgetManager().SetFromKey(std::stoi(key), val.get<double>());
        }
    }

    tracker.ClearDirty();
    return true;
}

} // namespace JsonIO
