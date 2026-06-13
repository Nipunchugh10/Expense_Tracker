#include "FilterCriteria.h"
#include "Utils.h"

bool FilterCriteria::Matches(const Expense& e) const {
    // Text search (description or category)
    if (!searchText.empty()) {
        if (!Utils::ContainsCI(e.GetDescription(), searchText) &&
            !Utils::ContainsCI(e.GetCategory(), searchText)) {
            return false;
        }
    }

    // Date range
    if (e.GetDate() < dateFrom || e.GetDate() > dateTo) {
        return false;
    }

    // Category filter
    if (!category.empty() && Utils::ToLower(e.GetCategory()) != Utils::ToLower(category)) {
        return false;
    }

    // Amount range
    if (e.GetAmount() < amountMin || e.GetAmount() > amountMax) {
        return false;
    }

    // Currency
    if (!currency.empty() && Utils::ToLower(e.GetCurrency()) != Utils::ToLower(currency)) {
        return false;
    }

    return true;
}

void FilterCriteria::Reset() {
    searchText.clear();
    dateFrom = {1900, 1, 1};
    dateTo   = {2100, 12, 31};
    category.clear();
    amountMin = 0.0;
    amountMax = 1e12;
    currency.clear();
}
