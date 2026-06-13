#include "Tabs/ExpensesTab.h"
#include "ExpenseTracker.h"
#include "imgui.h"
#include <cstring>
#include <algorithm>

void ExpensesTab::Render(ExpenseTracker& tracker) {
    ImGui::Spacing();

    if (ImGui::Button("+ Add Expense")) {
        openAddPopup = true;
        memset(addDesc, 0, sizeof(addDesc));
        addAmount = 0.0;
        Date today = Date::Today();
        addDate[0] = today.year;
        addDate[1] = today.month;
        addDate[2] = today.day;
        memset(addCategory, 0, sizeof(addCategory));
        strncpy(addCurrency, "INR", sizeof(addCurrency) - 1);
    }

    ImGui::Spacing();
    RenderFilterBar(tracker);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    RenderTable(tracker);

    RenderAddPopup(tracker);
    RenderEditPopup(tracker);
    RenderDeleteConfirm(tracker);
}

void ExpensesTab::RenderFilterBar(ExpenseTracker& tracker) {
    ImGui::Text("Filters:");
    ImGui::SameLine();

    ImGui::SetNextItemWidth(200);
    ImGui::InputTextWithHint("##Search", "Search...", searchBuf, sizeof(searchBuf));
    ImGui::SameLine();

    ImGui::SetNextItemWidth(160);
    if (ImGui::BeginCombo("##FilterCat", filterCategory[0] ? filterCategory : "All Categories")) {
        if (ImGui::Selectable("All Categories", filterCategory[0] == 0)) {
            memset(filterCategory, 0, sizeof(filterCategory));
        }
        for (auto& cat : tracker.GetCategories()) {
            if (ImGui::Selectable(cat.c_str(), strcmp(filterCategory, cat.c_str()) == 0)) {
                strncpy(filterCategory, cat.c_str(), sizeof(filterCategory) - 1);
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();

    ImGui::SetNextItemWidth(100);
    ImGui::InputDouble("##AmtMin", &filterAmountMin, 0, 0, "Min $%.0f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputDouble("##AmtMax", &filterAmountMax, 0, 0, "Max $%.0f");

    ImGui::SameLine();
    if (ImGui::Button("Reset Filters")) {
        memset(searchBuf, 0, sizeof(searchBuf));
        memset(filterCategory, 0, sizeof(filterCategory));
        filterAmountMin = 0.0;
        filterAmountMax = 1e12;
        filterDateFrom[0] = 1900; filterDateFrom[1] = 1; filterDateFrom[2] = 1;
        filterDateTo[0] = 2100; filterDateTo[1] = 12; filterDateTo[2] = 31;
    }
}

void ExpensesTab::RenderTable(ExpenseTracker& tracker) {
    FilterCriteria criteria;
    criteria.searchText = searchBuf;
    criteria.dateFrom = {filterDateFrom[0], filterDateFrom[1], filterDateFrom[2]};
    criteria.dateTo = {filterDateTo[0], filterDateTo[1], filterDateTo[2]};
    criteria.category = filterCategory;
    criteria.amountMin = filterAmountMin;
    criteria.amountMax = filterAmountMax;

    auto filtered = tracker.GetFilteredExpenses(criteria);

    if (sortCol >= 0) {
        auto cmp = [&](const Expense* a, const Expense* b) -> bool {
            int result = 0;
            switch (sortCol) {
                case 0: result = a->GetID() - b->GetID(); break;
                case 1: result = a->GetDate().ToInt() - b->GetDate().ToInt(); break;
                case 2: result = a->GetDescription().compare(b->GetDescription()); break;
                case 3: result = (a->GetAmount() < b->GetAmount()) ? -1 :
                                 (a->GetAmount() > b->GetAmount()) ? 1 : 0; break;
                case 4: result = a->GetCurrency().compare(b->GetCurrency()); break;
                case 5: result = a->GetCategory().compare(b->GetCategory()); break;
                default: break;
            }
            return sortAsc ? (result < 0) : (result > 0);
        };
        std::sort(filtered.begin(), filtered.end(), cmp);
    }

    ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                            ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable |
                            ImGuiTableFlags_ScrollY;
    float tableHeight = ImGui::GetContentRegionAvail().y - 25.0f;

    if (ImGui::BeginTable("##ExpenseTable", 7, flags, ImVec2(0, tableHeight))) {
        ImGui::TableSetupColumn("ID",          ImGuiTableColumnFlags_DefaultSort, 0, 0);
        ImGui::TableSetupColumn("Date",        0, 0, 1);
        ImGui::TableSetupColumn("Description", 0, 0, 2);
        ImGui::TableSetupColumn("Amount",      0, 0, 3);
        ImGui::TableSetupColumn("Currency",    0, 0, 4);
        ImGui::TableSetupColumn("Category",    0, 0, 5);
        ImGui::TableSetupColumn("Actions",     ImGuiTableColumnFlags_NoSort);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs()) {
            if (sortSpecs->SpecsDirty && sortSpecs->SpecsCount > 0) {
                sortCol = (int)sortSpecs->Specs[0].ColumnUserID;
                sortAsc = (sortSpecs->Specs[0].SortDirection == ImGuiSortDirection_Ascending);
                sortSpecs->SpecsDirty = false;
            }
        }

        for (auto* e : filtered) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", e->GetID());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", e->GetDate().ToString().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", e->GetDescription().c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", e->GetAmount());
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", e->GetCurrency().c_str());
            ImGui::TableSetColumnIndex(5); ImGui::Text("%s", e->GetCategory().c_str());
            ImGui::TableSetColumnIndex(6);

            ImGui::PushID(e->GetID());
            if (ImGui::SmallButton("Edit")) {
                editID = e->GetID();
                strncpy(editDesc, e->GetDescription().c_str(), sizeof(editDesc) - 1);
                editAmount = e->GetAmount();
                editDate[0] = e->GetDate().year;
                editDate[1] = e->GetDate().month;
                editDate[2] = e->GetDate().day;
                strncpy(editCategory, e->GetCategory().c_str(), sizeof(editCategory) - 1);
                strncpy(editCurrency, e->GetCurrency().c_str(), sizeof(editCurrency) - 1);
                openEditPopup = true;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Delete")) {
                deleteID = e->GetID();
                openDeleteConfirm = true;
            }
            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    ImGui::Text("%d expense(s) shown", (int)filtered.size());
}

void ExpensesTab::RenderAddPopup(ExpenseTracker& tracker) {
    if (openAddPopup) {
        ImGui::OpenPopup("Add Expense");
        openAddPopup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(560, 0), ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal("Add Expense", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Description", addDesc, sizeof(addDesc));
        ImGui::InputDouble("Amount", &addAmount, 1.0, 10.0, "%.2f");

        ImGui::SetNextItemWidth(120);
        ImGui::InputInt("Year", &addDate[0]);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(110);
        ImGui::InputInt("Mo##add", &addDate[1]);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(110);
        ImGui::InputInt("Day##add", &addDate[2]);

        ImGui::InputText("Category", addCategory, sizeof(addCategory));
        ImGui::InputText("Currency", addCurrency, sizeof(addCurrency));

        ImGui::Spacing();

        if (ImGui::Button("Add", ImVec2(130, 0))) {
            if (addDesc[0] && addAmount >= 0) {
                Date d = {addDate[0], addDate[1], addDate[2]};
                tracker.AddExpense(addDesc, addAmount, d,
                                   addCategory[0] ? addCategory : "General",
                                   addCurrency[0] ? addCurrency : "INR");
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(130, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ExpensesTab::RenderEditPopup(ExpenseTracker& tracker) {
    if (openEditPopup) {
        ImGui::OpenPopup("Edit Expense");
        openEditPopup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(560, 0), ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal("Edit Expense", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Editing Expense #%d", editID);
        ImGui::Separator();

        ImGui::InputText("Description##edit", editDesc, sizeof(editDesc));
        ImGui::InputDouble("Amount##edit", &editAmount, 1.0, 10.0, "%.2f");

        ImGui::SetNextItemWidth(120);
        ImGui::InputInt("Year##edit", &editDate[0]);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(110);
        ImGui::InputInt("Mo##edit2", &editDate[1]);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(110);
        ImGui::InputInt("Day##edit2", &editDate[2]);

        ImGui::InputText("Category##edit", editCategory, sizeof(editCategory));
        ImGui::InputText("Currency##edit", editCurrency, sizeof(editCurrency));

        ImGui::Spacing();

        if (ImGui::Button("Save", ImVec2(130, 0))) {
            Date d = {editDate[0], editDate[1], editDate[2]};
            tracker.UpdateExpense(editID, editDesc, editAmount, d,
                                  editCategory[0] ? editCategory : "General",
                                  editCurrency[0] ? editCurrency : "INR");
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##edit", ImVec2(130, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ExpensesTab::RenderDeleteConfirm(ExpenseTracker& tracker) {
    if (openDeleteConfirm) {
        ImGui::OpenPopup("Delete Expense?");
        openDeleteConfirm = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Delete Expense?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete expense #%d?", deleteID);
        ImGui::Text("This action cannot be undone.");
        ImGui::Spacing();

        if (ImGui::Button("Delete", ImVec2(130, 0))) {
            tracker.DeleteExpense(deleteID);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel##del", ImVec2(130, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
