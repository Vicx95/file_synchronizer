#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <regex>
#include <set>
#include <string>

#include <condition_variable>

#include "ftxui/component/component.hpp" // for Button, Horizontal, Renderer
#include <ftxui/dom/table.hpp>           // for Table, TableSelection

#include "model.hpp"

namespace fs = std::filesystem;

class View
{
public:
    View() = default;
    ~View() = default;

    enum class Message
    {
        Incorrect = 0,
        GiveFolder,
        DirExists,
        Interval,
        Remove,
        RemoveFile,
        PrintFiles,
        FolderEmpty
    };

    virtual void run(const fs::path &path) = 0;
    virtual void setModel(i_Model *ptr) = 0;
    void printMenu();
    void printOptions();
    void printDirectory(const fs::path &path);
    void printFiles(const std::set<fs::path> &fileDirList);

    void printMessage(Message m);

protected:
    std::atomic<bool> m_isExitRequested = false;

private:
    bool validateForPrinting(std::string name);
};
