#include "Application.h"
#include "imgui/imgui.h"
#include "logger.h"


/*
Logging System Additions:
- Added reusable logging system implemented as initialized singleton (Logger)
- Created a header Logger class
- Integrated a Dear ImGui debug console window to display log output during runtime
- Added file logging support to write all log messages to a persistent log file (app.log)
- Updated the main game loop to render the logging window each frame
*/


namespace ClassGame {
        //
        // our global variables
        //
        static bool g_showLogWindow = true;
        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            // Initialize logging system
//            Logger& logger = Logger::GetInstance();
//            logger.LogInfo("Game started successfully");
//            logger.LogGameEvent("Application initialized");
            Logger::GetInstance().Init("app.log");
            LOG_INFO("Game started successfully");
        }

        void ShowLogWindow(bool* p_open) {
            if (p_open) {
                Logger::GetInstance().DrawImGuiWindow(p_open);
            } else if (g_showLogWindow) {
                Logger::GetInstance().DrawImGuiWindow(&g_showLogWindow);
            }
        }

        void ToggleLogWindow() {
            g_showLogWindow = !g_showLogWindow;
        }

        bool IsLogWindowVisible() {
            return g_showLogWindow;
        }


        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
            ImGui::DockSpaceOverViewport();
            ImGui::ShowDemoWindow();

            ImGui::Begin("ImGui Log Demo");
            ImGui::LogButtons();

            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }

            if (ImGui::Button("Write a logger message")) {
                LOG_WARN("Button pressed: writing a warning to console + file");
            }

            ImGui::End();

            ShowLogWindow();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
        }
}
