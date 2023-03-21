#pragma once

#include "model.hpp"
#include "view.hpp"

class ViewFTXuserInterface : public View
{
public:
    void run(const fs::path &path);

    ftxui::Component createButtons(std::deque<bool *> &showButtons, std::condition_variable &cv, std::atomic<bool> &refresh_ui_continue, std::vector<std::string> &dirNamesX, ftxui::ScreenInteractive &screen, const fs::path &path);
    void hideMenuButtons(std::deque<bool *> &showButtons);
    void refreshDir(std::vector<std::string> &dirNames, const fs::path &path);
    void generateColorTable(ftxui::Table *table);
    ftxui::Element printLogoGraph();
    std::string get_permission_string(fs::perms permission);
    void setModel(i_Model *ptr);

    std::vector<std::vector<std::string>> printDir(const fs::path &path);
    std::vector<std::vector<std::string>> printAllFiles(const fs::path &path);

    i_Model *model;

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