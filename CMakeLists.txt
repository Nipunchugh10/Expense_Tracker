cmake_minimum_required(VERSION 3.16)

set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)

project(Expense_Tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── GLFW (from source, no extras) ──────────────────────────────────
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# ── ImGui (static library) ─────────────────────────────────────────
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)

# ── ImPlot (static library) ────────────────────────────────────────
set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/third_party/implot)
add_library(implot STATIC
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)
target_include_directories(implot PUBLIC ${IMPLOT_DIR})
target_link_libraries(implot PUBLIC imgui)

# ── nlohmann/json (header-only) ────────────────────────────────────
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
    ${CMAKE_SOURCE_DIR}/third_party
)

# ── Application ────────────────────────────────────────────────────
set(APP_SOURCES
    src/main.cpp
    src/Expense.cpp
    src/ExpenseTracker.cpp
    src/Utils.cpp
    src/Budget.cpp
    src/FilterCriteria.cpp
    src/RecurringRule.cpp
    src/JsonIO.cpp
    src/CsvIO.cpp
    src/App.cpp
    src/Tabs/DashboardTab.cpp
    src/Tabs/ExpensesTab.cpp
    src/Tabs/BudgetTab.cpp
    src/Tabs/AnalyticsTab.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${APP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    implot
    nlohmann_json
    opengl32
    gdi32
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_options(${PROJECT_NAME} PRIVATE -static)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
