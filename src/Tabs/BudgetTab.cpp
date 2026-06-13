#include "Tabs/BudgetTab.h"
#include "ExpenseTracker.h"
#include "imgui.h"
#include <cstdio>

static const char* monthNames[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

void BudgetTab::LoadBudgets(ExpenseTracker& tracker) {
    for (int m = 0; m < 12; m++) {
        budgetInputs[m] = tracker.GetBudgetManager().GetBudget(selectedYear, m + 1);
    }
}

void BudgetTab::Render(ExpenseTracker& tracker) {
    ImGui::Spacing();

    int prevYear = selectedYear;
    ImGui::SetNextItemWidth(150);
    ImGui::InputInt("Year", &selectedYear);

    if (!initialized || selectedYear != prevYear) {
        LoadBudgets(tracker);
        initialized = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Set monthly budgets and track spending for %d:", selectedYear);
    ImGui::Spacing();

    auto monthlyTotals = tracker.GetMonthlyTotals(selectedYear);

    for (int m = 0; m < 12; m++) {
        int month = m + 1;
        double spent = monthlyTotals[month];
        double budget = budgetInputs[m];

        ImGui::PushID(m);

        ImGui::Text("%-10s", monthNames[m]);
        ImGui::SameLine(160);

        ImGui::SetNextItemWidth(150);
        if (ImGui::InputDouble("##budget", &budgetInputs[m], 0, 0, "$%.2f")) {
            tracker.GetBudgetManager().SetBudget(selectedYear, month, budgetInputs[m]);
        }

        ImGui::SameLine(340);

        float progress = 0.0f;
        ImVec4 barColor;

        if (budget > 0.0) {
            progress = (float)(spent / budget);

            if (progress > 1.0f) {
                barColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
            } else if (progress >= 0.8f) {
                barColor = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
            } else {
                barColor = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
            }

            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, barColor);

            char overlay[64];
            snprintf(overlay, sizeof(overlay), "$%.2f / $%.2f", spent, budget);
            ImGui::ProgressBar(progress > 1.0f ? 1.0f : progress,
                               ImVec2(ImGui::GetContentRegionAvail().x - 10, 22),
                               overlay);

            ImGui::PopStyleColor();
        } else {
            char overlay[64];
            snprintf(overlay, sizeof(overlay), "$%.2f spent (no budget)", spent);
            ImGui::ProgressBar(0.0f,
                               ImVec2(ImGui::GetContentRegionAvail().x - 10, 22),
                               overlay);
        }

        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    double totalBudget = 0.0;
    double totalSpent = 0.0;
    for (int m = 0; m < 12; m++) {
        totalBudget += budgetInputs[m];
        totalSpent += monthlyTotals[m + 1];
    }

    ImGui::Text("Yearly Budget: $%.2f | Yearly Spent: $%.2f | Remaining: $%.2f",
                totalBudget, totalSpent, totalBudget - totalSpent);
}
