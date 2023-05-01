#pragma once

#include "model.hpp"
#include "utils.hpp"
#include "view.hpp"

#include <atomic>
#include <chrono>
#include <functional>
#include <optional>
#include <regex>
#include <unordered_map>

using namespace std::chrono_literals;

class Controller
{
public:
    using pVoid = std::function<void(Controller *)>;

    Controller();
    explicit Controller(std::unique_ptr<View> view, std::unique_ptr<Model> model) noexcept;
    ~Controller() = default;

    void run();

private:
    class Handlers
    {
    public:
        friend class Controller;

    private:
        enum class Action
        {
            AddDir = 1,
            RemoveDir = 2,
            RemoveFile = 3,
            PrintDir = 4,
            PrintFiles = 5,
            SetIntervalTime = 6,
            StartSync = 7,
            StopSync = 8,
            ForceSync = 9,
            ReadConfig = 10,
            SaveConfig = 11,
            SetupStreaming = 12,
            SetupNetwork = 13,
            Exit = 0
        };

        std::unordered_map<Handlers::Action, pVoid, utils::EnumClassHash> m_handlerMap;
    };

    void waitForButton();
    std::optional<std::string> getKeyboardInput(std::regex keyRegex = std::regex("^.{1,50}"));

    void init();

    void process(auto fun);

    void addDirectory();
    void removeDirectory();
    void removeFile();
    void printDirectory();
    void printFiles();
    void setIntervalTime();
    void startSync();
    void stopSync();
    void forceSync();
    void readConfig();
    void saveConfig();
    void setupStreaming();
    void setupNetwork();
    void exit();

    std::atomic<bool> m_isExitRequested = false;
    static constexpr std::chrono::milliseconds Config_UISleepFor = 2000ms;

    std::unique_ptr<View> m_view = nullptr;
    std::unique_ptr<Model> m_model = nullptr;
    Handlers h;
};
