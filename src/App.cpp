#include "App.h"
#include "JsonIO.h"
#include "CsvIO.h"
#include "imgui.h"
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

namespace fs = std::filesystem;

App::App() = default;

void App::Init() {
    // Try loading saved data
    if (fs::exists(dataPath)) {
        JsonIO::Load(tracker, dataPath);
        statusMessage = "Data loaded from " + dataPath;
    } else {
        statusMessage = "No saved data found. Starting fresh.";
    }
    statusTimer = 3.0f;
}

void App::Render() {
    ImGuiIO& io = ImGui::GetIO();

    // Full-window panel
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("##MainWindow", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

    RenderMenuBar();

    // Tab bar
    if (ImGui::BeginTabBar("##Tabs")) {
        if (ImGui::BeginTabItem("Dashboard")) {
            dashboardTab.Render(tracker);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Expenses")) {
            expensesTab.Render(tracker);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Budget")) {
            budgetTab.Render(tracker);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Analytics")) {
            analyticsTab.Render(tracker);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    RenderStatusBar();

    ImGui::End();

    // Auto-save
    autoSaveTimer += io.DeltaTime;
    if (autoSaveTimer >= autoSaveInterval && tracker.HasChanges()) {
        AutoSave();
        autoSaveTimer = 0.0f;
    }

    // Status message timer
    if (statusTimer > 0.0f) {
        statusTimer -= io.DeltaTime;
    }
}

void App::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                if (JsonIO::Save(tracker, dataPath)) {
                    tracker.ClearDirty();
                    statusMessage = "Saved successfully.";
                    statusTimer = 3.0f;
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Import CSV...")) {
                #ifdef _WIN32
                char filename[MAX_PATH] = {};
                OPENFILENAMEA ofn = {};
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
                if (GetOpenFileNameA(&ofn)) {
                    if (CsvIO::Import(tracker, filename)) {
                        statusMessage = "CSV imported successfully.";
                    } else {
                        statusMessage = "Failed to import CSV.";
                    }
                    statusTimer = 3.0f;
                }
                #endif
            }
            if (ImGui::MenuItem("Export CSV...")) {
                #ifdef _WIN32
                char filename[MAX_PATH] = "expenses.csv";
                OPENFILENAMEA ofn = {};
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
                ofn.lpstrDefExt = "csv";
                if (GetSaveFileNameA(&ofn)) {
                    if (CsvIO::Export(tracker, filename)) {
                        statusMessage = "CSV exported successfully.";
                    } else {
                        statusMessage = "Failed to export CSV.";
                    }
                    statusTimer = 3.0f;
                }
                #endif
            }
            if (ImGui::MenuItem("Export JSON...")) {
                #ifdef _WIN32
                char filename[MAX_PATH] = "expenses.json";
                OPENFILENAMEA ofn = {};
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
                ofn.lpstrDefExt = "json";
                if (GetSaveFileNameA(&ofn)) {
                    if (JsonIO::Save(tracker, filename)) {
                        statusMessage = "JSON exported successfully.";
                    } else {
                        statusMessage = "Failed to export JSON.";
                    }
                    statusTimer = 3.0f;
                }
                #endif
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                // Save before exit
                if (tracker.HasChanges()) {
                    JsonIO::Save(tracker, dataPath);
                }
                exit(0);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Ctrl+S shortcut
    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
        if (JsonIO::Save(tracker, dataPath)) {
            tracker.ClearDirty();
            statusMessage = "Saved successfully.";
            statusTimer = 3.0f;
        }
    }
}

void App::RenderStatusBar() {
    ImGui::Separator();
    if (statusTimer > 0.0f && !statusMessage.empty()) {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "%s", statusMessage.c_str());
    } else {
        ImGui::Text("%d expenses | %s",
                    tracker.GetExpenseCount(),
                    tracker.HasChanges() ? "Unsaved changes" : "All saved");
    }
}

void App::AutoSave() {
    if (JsonIO::Save(tracker, dataPath)) {
        tracker.ClearDirty();
        statusMessage = "Auto-saved.";
        statusTimer = 2.0f;
    }
}

void App::Shutdown() {
    if (tracker.HasChanges()) {
        JsonIO::Save(tracker, dataPath);
    }
}
