#include "Tabs/AnalyticsTab.h"
#include "ExpenseTracker.h"
#include "imgui.h"
#include "implot.h"
#include <vector>
#include <string>
#include <algorithm>

void AnalyticsTab::Render(ExpenseTracker& tracker) {
    ImGui::Spacing();

    ImGui::SetNextItemWidth(150);
    ImGui::InputInt("Year", &selectedYear);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    float halfWidth = (ImGui::GetContentRegionAvail().x - 10.0f) / 2.0f;

    // ── Left column: Category breakdown table ─────────────────────
    ImGui::BeginChild("##AnalyticsLeft", ImVec2(halfWidth, 0), false);

    ImGui::Text("Category Breakdown (%d)", selectedYear);
    ImGui::Spacing();

    auto breakdown = tracker.GetCategoryBreakdown(selectedYear);
    double yearTotal = tracker.GetYearlyTotal(selectedYear);

    if (ImGui::BeginTable("##CatBreakdown", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Total");
        ImGui::TableSetupColumn("Percentage");
        ImGui::TableHeadersRow();

        // Sort by amount descending
        std::vector<std::pair<std::string, double>> sorted(breakdown.begin(), breakdown.end());
        std::sort(sorted.begin(), sorted.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        for (auto& [cat, val] : sorted) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", cat.empty() ? "Uncategorized" : cat.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("$%.2f", val);
            ImGui::TableSetColumnIndex(2);
            float pct = yearTotal > 0 ? (float)(val / yearTotal * 100.0) : 0.0f;
            ImGui::Text("%.1f%%", pct);
        }

        // Total row
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "TOTAL");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "$%.2f", yearTotal);
        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "100%%");

        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Currency breakdown
    ImGui::Text("Currency Summary");
    auto currBreakdown = tracker.GetCurrencyBreakdown(selectedYear);
    if (ImGui::BeginTable("##CurrBreakdown", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Currency");
        ImGui::TableSetupColumn("Total");
        ImGui::TableHeadersRow();

        for (auto& [cur, val] : currBreakdown) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", cur.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f", val);
        }

        ImGui::EndTable();
    }

    ImGui::EndChild();

    ImGui::SameLine();

    // ── Right column: Charts and Top 10 ───────────────────────────
    ImGui::BeginChild("##AnalyticsRight", ImVec2(halfWidth, 0), false);

    // Monthly trend chart
    ImGui::Text("Monthly Trend (%d)", selectedYear);
    auto monthlyTotals = tracker.GetMonthlyTotals(selectedYear);
    double trendData[12];
    const char* barLabels[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    double positions[12];
    for (int i = 0; i < 12; i++) {
        trendData[i] = monthlyTotals[i + 1];
        positions[i] = i;
    }

    if (ImPlot::BeginPlot("##TrendChart", ImVec2(-1, 340))) {
        ImPlot::SetupAxes("Month", "Amount ($)");
        ImPlot::SetupAxisTicks(ImAxis_X1, positions, 12, barLabels);
        ImPlot::PlotBars("Spending", trendData, 12, 0.67);
        ImPlot::PlotLine("Trend", trendData, 12);
        ImPlot::EndPlot();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Top 10 expenses
    ImGui::Text("Top 10 Most Expensive Items (%d)", selectedYear);

    auto topExpenses = tracker.GetTopExpenses(selectedYear, 10);
    if (ImGui::BeginTable("##Top10", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("#");
        ImGui::TableSetupColumn("Description");
        ImGui::TableSetupColumn("Amount");
        ImGui::TableSetupColumn("Date");
        ImGui::TableHeadersRow();

        int rank = 1;
        for (auto* e : topExpenses) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", rank++);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", e->GetDescription().c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("$%.2f", e->GetAmount());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", e->GetDate().ToString().c_str());
        }

        ImGui::EndTable();
    }

    if (topExpenses.empty()) {
        ImGui::TextDisabled("No expenses for this year.");
    }

    ImGui::EndChild();
}
