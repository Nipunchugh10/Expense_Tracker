#include "Tabs/DashboardTab.h"
#include "ExpenseTracker.h"
#include "JsonIO.h"
#include "imgui.h"
#include "implot.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

static const char* monthNames[] = {
    "All", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

void DashboardTab::Render(ExpenseTracker& tracker) {
    ImGui::Spacing();

    // Top bar: Year / Month selector + Reset button
    ImGui::SetNextItemWidth(130);
    ImGui::InputInt("Year", &selectedYear);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(160);
    ImGui::Combo("Month", &selectedMonth, monthNames, 13);

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 180);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.05f, 0.05f, 1.0f));
    if (ImGui::Button("Reset Everything", ImVec2(170, 0))) {
        openResetConfirm = true;
    }
    ImGui::PopStyleColor(3);

    // Reset confirmation modal
    if (openResetConfirm) {
        ImGui::OpenPopup("Reset All Data?");
        openResetConfirm = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Reset All Data?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("This will permanently delete ALL data:");
        ImGui::BulletText("All expenses");
        ImGui::BulletText("All recurring rules");
        ImGui::BulletText("All budgets");
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "This action cannot be undone!");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.05f, 0.05f, 1.0f));
        if (ImGui::Button("Yes, Reset Everything", ImVec2(200, 0))) {
            tracker.GetExpenses().clear();
            tracker.GetRecurringRules().clear();
            tracker.GetBudgetManager().Clear();
            tracker.SetNextExpenseID(1);
            tracker.SetNextRuleID(1);
            tracker.ClearDirty();
            JsonIO::Save(tracker, "data/expenses.json");
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Summary cards
    double yearTotal = tracker.GetYearlyTotal(selectedYear);
    double monthTotal = (selectedMonth > 0)
        ? tracker.GetMonthlyTotal(selectedYear, selectedMonth) : yearTotal;

    double budgetRemaining = 0.0;
    if (selectedMonth > 0 && tracker.GetBudgetManager().HasBudget(selectedYear, selectedMonth)) {
        double budget = tracker.GetBudgetManager().GetBudget(selectedYear, selectedMonth);
        budgetRemaining = budget - monthTotal;
    }

    int expCount = 0;
    for (auto& e : tracker.GetExpensesRef()) {
        if (e.GetYear() == selectedYear) {
            if (selectedMonth == 0 || e.GetMonth() == selectedMonth)
                expCount++;
        }
    }

    // Cards row
    float cardWidth = (ImGui::GetContentRegionAvail().x - 30.0f) / 4.0f;

    auto DrawCard = [&](const char* label, const char* value, ImVec4 color) {
        ImGui::BeginChild(label, ImVec2(cardWidth, 58), true);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", label);
        ImGui::TextColored(color, "%s", value);
        ImGui::EndChild();
    };

    char buf[64];
    snprintf(buf, sizeof(buf), "$%.2f", yearTotal);
    DrawCard("Year Total", buf, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
    ImGui::SameLine();

    snprintf(buf, sizeof(buf), "$%.2f", monthTotal);
    DrawCard("Period Total", buf, ImVec4(0.4f, 0.8f, 1.0f, 1.0f));
    ImGui::SameLine();

    if (selectedMonth > 0 && tracker.GetBudgetManager().HasBudget(selectedYear, selectedMonth)) {
        snprintf(buf, sizeof(buf), "$%.2f", budgetRemaining);
        ImVec4 budgetColor = budgetRemaining >= 0
            ? ImVec4(0.4f, 0.9f, 0.4f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
        DrawCard("Budget Remaining", buf, budgetColor);
    } else {
        DrawCard("Budget Remaining", "N/A", ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    ImGui::SameLine();

    snprintf(buf, sizeof(buf), "%d", expCount);
    DrawCard("Expense Count", buf, ImVec4(0.8f, 0.6f, 1.0f, 1.0f));

    ImGui::Spacing();

    // Charts side by side
    float halfWidth = (ImGui::GetContentRegionAvail().x - 10.0f) / 2.0f;
    float chartSectionH = 360;
    float plotH = 310;

    // Category Pie Chart
    ImGui::BeginChild("##PieSection", ImVec2(halfWidth, chartSectionH), true);
    ImGui::Text("Category Breakdown");

    auto breakdown = tracker.GetCategoryBreakdown(selectedYear, selectedMonth);
    if (!breakdown.empty()) {
        std::vector<std::string> labels;
        std::vector<double> values;
        for (auto& [cat, val] : breakdown) {
            labels.push_back(cat.empty() ? "Uncategorized" : cat);
            values.push_back(val);
        }

        std::vector<const char*> labelPtrs;
        for (auto& l : labels) labelPtrs.push_back(l.c_str());

        if (ImPlot::BeginPlot("##PieChart", ImVec2(-1, plotH), ImPlotFlags_Equal | ImPlotFlags_NoMouseText)) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
            ImPlot::SetupAxesLimits(0, 1, 0, 1);
            ImPlot::PlotPieChart(labelPtrs.data(), values.data(),
                                 (int)values.size(), 0.5, 0.5, 0.4, "%.1f", 90);
            ImPlot::EndPlot();
        }
    } else {
        ImGui::TextDisabled("No data for this period.");
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Monthly Spending Bar Chart
    ImGui::BeginChild("##BarSection", ImVec2(halfWidth, chartSectionH), true);
    ImGui::Text("Monthly Spending (%d)", selectedYear);

    auto monthlyTotals = tracker.GetMonthlyTotals(selectedYear);
    double barData[12];
    const char* barLabels[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    double positions[12];
    for (int i = 0; i < 12; i++) {
        barData[i] = monthlyTotals[i + 1];
        positions[i] = i;
    }

    if (ImPlot::BeginPlot("##BarChart", ImVec2(-1, plotH))) {
        ImPlot::SetupAxes("Month", "Amount ($)");
        ImPlot::SetupAxisTicks(ImAxis_X1, positions, 12, barLabels);
        ImPlot::PlotBars("Spending", barData, 12, 0.67);
        ImPlot::EndPlot();
    }
    ImGui::EndChild();

    ImGui::Spacing();

    // Recent expenses (last 10)
    ImGui::Text("Recent Expenses");
    ImGui::Separator();

    auto allExpenses = tracker.GetAllExpenses();
    std::sort(allExpenses.begin(), allExpenses.end(),
              [](const Expense* a, const Expense* b) {
                  return a->GetDate().ToInt() > b->GetDate().ToInt();
              });

    if (ImGui::BeginTable("##RecentExpenses", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Date");
        ImGui::TableSetupColumn("Description");
        ImGui::TableSetupColumn("Amount");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Currency");
        ImGui::TableHeadersRow();

        int shown = 0;
        for (auto* e : allExpenses) {
            if (shown >= 10) break;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", e->GetDate().ToString().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", e->GetDescription().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", e->GetAmount());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", e->GetCategory().c_str());
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", e->GetCurrency().c_str());
            shown++;
        }
        ImGui::EndTable();
    }

    if (allExpenses.empty()) {
        ImGui::TextDisabled("No expenses yet. Add some from the Expenses tab.");
    }
}
