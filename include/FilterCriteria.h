#pragma once
#include <string>
#include "Expense.h"

struct FilterCriteria {
    std::string searchText;
    Date        dateFrom = {1900, 1, 1};
    Date        dateTo   = {2100, 12, 31};
    std::string category;       // empty = all
    double      amountMin = 0.0;
    double      amountMax = 1e12;
    std::string currency;       // empty = all

    bool Matches(const Expense& e) const;
    void Reset();
};
