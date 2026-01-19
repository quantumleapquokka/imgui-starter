#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include "imgui/imgui.h"

class Logger {

public:
    enum class Level {Info, Warn , Error};

    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void Init(const std::string& filePath) {
        if (m_initialized) {
            return;
        }
        m_filePath = filePath;
        m_out.open(m_filePath, std::ios::out | std::ios::app);
        m_initialized = true;

        Log(Level::Info, "Logger initialized. Output file: %s", m_filePath.c_str());
    }

    void Log(Level level, const char* fmt, ...) {
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);
        std::string line =  Prefix(level) + msg;
        m_lines.push_back(line);
        if (m_out.is_open()) {
            m_out << line << "\n";
            m_out.flush();
        }
    }

    void DrawImGuiWindow(bool* p_open = nullptr) {
        if (!ImGui::Begin("Debug Console", p_open)) {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Clear")) {
            m_lines.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("[+] test log")) {
            Log(Level::Info, "Test log line from button");
        }

        ImGui::Separator();

        ImGui::BeginChild("log_scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (const std::string& s : m_lines) {
            ImGui::TextUnformatted(s.c_str());
        }
        if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();

        ImGui::End();
    }

private: 
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string Prefix(Level level) {
        switch(level) {
            case Level::Info: return "[INFO] ";
            case Level::Warn: return "[WARNING] ";
            case Level::Error: return "[ERROR] ";
        } return "[INFO] ";
    }

private:
    bool m_initialized = false;
    bool m_autoScroll = true;
    std::string m_filePath;
    std::ofstream m_out;

    std::vector<std::string> m_lines;

};

#define LOG_INFO(...)  Logger::GetInstance().Log(Logger::Level::Info,  __VA_ARGS__)
#define LOG_WARN(...)  Logger::GetInstance().Log(Logger::Level::Warn,  __VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().Log(Logger::Level::Error, __VA_ARGS__)









