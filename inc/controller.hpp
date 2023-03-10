#pragma once

#include "model.hpp"
#include "view.hpp"
#include <unordered_map>

using pVoid = std::function<void()>;

class Controller
{
public:
    Controller(View *view, Model *model);

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
            readConfig = 10,
            saveConfig = 11,
            Exit = 0
        };

        struct EnumClassHash
        {
            template <typename T>
            std::size_t operator()(T t) const
            {
                return static_cast<std::size_t>(t);
            }
        };

        std::unordered_map<Handlers::Action, pVoid, EnumClassHash> m_handlerMap;
    };

    void
    waitForButton();

    void init();

    void addDirectory(std::istream &std_input);
    void removeDirectory();
    void removeFile();
    void printDirectory();
    void printFiles();
    void setIntervalTime(std::istream &std_input);
    void startSync();
    void stopSync();
    void forceSync();
    void readConfig();
    void saveConfig();
    bool exit();

    bool m_isExitRequested = false;

    View *m_view;
    Model *m_model;
    Handlers h;
};
