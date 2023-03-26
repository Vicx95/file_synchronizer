#pragma once

#include "model.hpp"
#include "view.hpp"

#include <condition_variable>

#include "ftxui/component/component.hpp" // for Button, Horizontal, Renderer
#include <ftxui/dom/table.hpp>           // for Table, TableSelection

class ViewFTXuserInterface : public View
{
public:
    void run(const fs::path &path) override;
    void setModel(i_Model *ptr) override; 


    ftxui::Component createButtons(std::deque<bool *> &showButtons, std::condition_variable &cv, std::atomic<bool> &refresh_ui_continue, std::vector<std::string> &dirNamesX, ftxui::ScreenInteractive &screen, const fs::path &path);
    void hideMenuButtons(std::deque<bool *> &showButtons);
    void refreshDir(std::vector<std::string> &dirNames, const fs::path &path);
    void refreshFile(std::vector<std::string> &dirNames, std::vector<std::vector<std::string>> &filesNames, const fs::path &path);

    void generateColorTable(ftxui::Table *table);
    ftxui::Element printLogoGraph();
    std::string get_permission_string(fs::perms permission);

    std::vector<std::vector<std::string>> printDir(const fs::path &path);
    std::vector<std::vector<std::string>> printAllFiles(const fs::path &path);

private:
    i_Model *m_model = nullptr;

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