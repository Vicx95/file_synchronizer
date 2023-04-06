#pragma once

#include "model.hpp"
#include "view.hpp"

#include <condition_variable>
#include <sstream>

#include <ftxui/component/captured_mouse.hpp>     // for ftxui
#include <ftxui/component/component.hpp>          // for Button, Horizontal, Renderer
#include <ftxui/component/component_base.hpp>     // for ComponentBase
#include <ftxui/component/screen_interactive.hpp> // for ScreenInteractive
#include <ftxui/dom/elements.hpp>                 // for separator, gauge, text, Element, operator|, vbox, border
#include <ftxui/dom/table.hpp>                    // for Table, TableSelection

using namespace ftxui;
using namespace std::chrono_literals;
namespace fs = std::filesystem;

class ViewFTXuserInterface : public View
{
public:
    virtual ~ViewFTXuserInterface();
    void run(const fs::path &path) override;
    void setModel(i_Model *ptr) override;
    std::string getTypeUI() override; 

    ftxui::Component createButtons(std::deque<bool *> &showButtons, std::condition_variable &cv, std::atomic<bool> &refresh_ui_continue, ftxui::ScreenInteractive &screen, bool &enterReadConfig);
    void hideMenuButtons(std::deque<bool *> &showButtons);
    void refreshDir(std::vector<std::string> &dirNames, const fs::path &path);
    void refreshFile(std::vector<std::string> &dirNames, std::vector<std::vector<std::string>> &filesNames, const fs::path &path);

    void generateColorTable(ftxui::Table *table);
    ftxui::Element printLogoGraph();
    std::string get_permission_string(fs::perms permission);
    void synchronizationFTX(std::mutex& cv_mutex, std::condition_variable& cv, ftxui::ScreenInteractive& screen, unsigned int& shift, unsigned int& interval, std::atomic<bool>& refresh_ui_continue);

    std::vector<std::vector<std::string>> printDir(const fs::path &path);
    std::vector<std::vector<std::string>> printAllFiles(const fs::path &path);

private:
    i_Model *m_model = nullptr;
    std::unique_ptr<i_Timer> m_FTXsyncTimer = std::make_unique<Timer>();
    /*
            void printDirectory();
            void printFiles();
            void removeFile();
            std::string get_permission_string(fs::perms permission);
            void generateColorTable(ftxui::Table* table);
            void refreshDir(std::vector<std::string> &dirNames);
            void refreshFile(std::vector<std::string> &dirNames, std::vector<std::vector<std::string>> &filesNames);
            void hideMenuButtons(std::deque<bool*> &showButtons);
            */
};