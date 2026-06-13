#include "ExpenseTracker.h"
#include "Utils.h"
#include <algorithm>
#include <set>

ExpenseTracker::ExpenseTracker() = default;

// ── Expense CRUD ──────────────────────────────────────────────────

void ExpenseTracker::AddExpense(const std::string& desc, double amount, const Date& date,
                                const std::string& category, const std::string& currency,
                                int recurringRuleId) {
    expenses.emplace_back(nextExpenseID++, desc, amount, date, category, currency, recurringRuleId);
    dirty = true;
}

void ExpenseTracker::UpdateExpense(int id, const std::string& desc, double amount,
                                    const Date& date, const std::string& category,
                                    const std::string& currency) {
    for (auto& e : expenses) {
        if (e.GetID() == id) {
            e.SetDescription(desc);
            e.SetAmount(amount);
            e.SetDate(date);
            e.SetCategory(category);
            e.SetCurrency(currency);
            dirty = true;
            return;
        }
    }
}

bool ExpenseTracker::DeleteExpense(int id) {
    for (auto it = expenses.begin(); it != expenses.end(); ++it) {
        if (it->GetID() == id) {
            expenses.erase(it);
            dirty = true;
            return true;
        }
    }
    return false;
}

// ── Recurring rule CRUD ───────────────────────────────────────────

void ExpenseTracker::AddRecurringRule(const RecurringRule& rule) {
    RecurringRule r = rule;
    // Assign ID if not set
    if (r.GetID() == 0) {
        // Use a simple method: set ID from nextRuleID
    }
    recurringRules.push_back(r);
    dirty = true;
}

void ExpenseTracker::UpdateRecurringRule(const RecurringRule& rule) {
    for (auto& r : recurringRules) {
        if (r.GetID() == rule.GetID()) {
            r = rule;
            dirty = true;
            return;
        }
    }
}

bool ExpenseTracker::DeleteRecurringRule(int id) {
    for (auto it = recurringRules.begin(); it != recurringRules.end(); ++it) {
        if (it->GetID() == id) {
            recurringRules.erase(it);
            dirty = true;
            return true;
        }
    }
    return false;
}

void ExpenseTracker::GenerateRecurringExpenses(const Date& upTo) {
    for (auto& rule : recurringRules) {
        if (!rule.IsActive()) continue;
        Date rangeStart = rule.GetStartDate();
        auto dates = rule.GetOccurrences(rangeStart, upTo);
        for (auto& d : dates) {
            // Check if expense already exists for this rule and date
            bool exists = false;
            for (auto& e : expenses) {
                if (e.GetRecurringRuleId() == rule.GetID() && e.GetDate() == d) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                AddExpense(rule.GetDescription(), rule.GetAmount(), d,
                           rule.GetCategory(), rule.GetCurrency(), rule.GetID());
            }
        }
    }
}

// ── Queries ───────────────────────────────────────────────────────

std::vector<const Expense*> ExpenseTracker::GetFilteredExpenses(const FilterCriteria& criteria) const {
    std::vector<const Expense*> result;
    for (auto& e : expenses) {
        if (criteria.Matches(e)) {
            result.push_back(&e);
        }
    }
    return result;
}

std::vector<const Expense*> ExpenseTracker::GetAllExpenses() const {
    std::vector<const Expense*> result;
    for (auto& e : expenses) {
        result.push_back(&e);
    }
    return result;
}

const Expense* ExpenseTracker::FindExpenseByID(int id) const {
    for (auto& e : expenses) {
        if (e.GetID() == id) return &e;
    }
    return nullptr;
}

// ── Analytics ─────────────────────────────────────────────────────

double ExpenseTracker::GetMonthlyTotal(int year, int month) const {
    double total = 0.0;
    for (auto& e : expenses) {
        if (e.GetYear() == year && e.GetMonth() == month) {
            total += e.GetAmount();
        }
    }
    return total;
}

double ExpenseTracker::GetYearlyTotal(int year) const {
    double total = 0.0;
    for (auto& e : expenses) {
        if (e.GetYear() == year) {
            total += e.GetAmount();
        }
    }
    return total;
}

std::map<std::string, double> ExpenseTracker::GetCategoryBreakdown(int year, int month) const {
    std::map<std::string, double> breakdown;
    for (auto& e : expenses) {
        if (e.GetYear() == year && (month == 0 || e.GetMonth() == month)) {
            breakdown[e.GetCategory()] += e.GetAmount();
        }
    }
    return breakdown;
}

std::map<int, double> ExpenseTracker::GetMonthlyTotals(int year) const {
    std::map<int, double> totals;
    for (int m = 1; m <= 12; m++) totals[m] = 0.0;
    for (auto& e : expenses) {
        if (e.GetYear() == year) {
            totals[e.GetMonth()] += e.GetAmount();
        }
    }
    return totals;
}

std::vector<const Expense*> ExpenseTracker::GetTopExpenses(int year, int count) const {
    std::vector<const Expense*> result;
    for (auto& e : expenses) {
        if (e.GetYear() == year) {
            result.push_back(&e);
        }
    }
    std::sort(result.begin(), result.end(),
              [](const Expense* a, const Expense* b) {
                  return a->GetAmount() > b->GetAmount();
              });
    if ((int)result.size() > count) result.resize(count);
    return result;
}

std::map<std::string, double> ExpenseTracker::GetCurrencyBreakdown(int year) const {
    std::map<std::string, double> breakdown;
    for (auto& e : expenses) {
        if (e.GetYear() == year) {
            breakdown[e.GetCurrency()] += e.GetAmount();
        }
    }
    return breakdown;
}

// ── Dropdown data ─────────────────────────────────────────────────

std::vector<std::string> ExpenseTracker::GetCategories() const {
    std::set<std::string> cats;
    for (auto& e : expenses) {
        if (!e.GetCategory().empty()) cats.insert(e.GetCategory());
    }
    return {cats.begin(), cats.end()};
}

std::vector<std::string> ExpenseTracker::GetCurrencies() const {
    std::set<std::string> curs;
    for (auto& e : expenses) {
        if (!e.GetCurrency().empty()) curs.insert(e.GetCurrency());
    }
    if (curs.empty()) curs.insert("USD");
    return {curs.begin(), curs.end()};
}

// ── Sorting ───────────────────────────────────────────────────────

void ExpenseTracker::SortExpenses(SortField field, SortOrder order) {
    auto cmp = [&](const Expense& a, const Expense& b) -> bool {
        int result = 0;
        switch (field) {
            case SortField::ID:
                result = a.GetID() - b.GetID(); break;
            case SortField::Date:
                result = a.GetDate().ToInt() - b.GetDate().ToInt(); break;
            case SortField::Description:
                result = a.GetDescription().compare(b.GetDescription()); break;
            case SortField::Amount:
                result = (a.GetAmount() < b.GetAmount()) ? -1 : (a.GetAmount() > b.GetAmount() ? 1 : 0); break;
            case SortField::Category:
                result = a.GetCategory().compare(b.GetCategory()); break;
            case SortField::Currency:
                result = a.GetCurrency().compare(b.GetCurrency()); break;
        }
        return (order == SortOrder::Ascending) ? (result < 0) : (result > 0);
    };
    std::sort(expenses.begin(), expenses.end(), cmp);
}
