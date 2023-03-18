#pragma once

#include "..//inc/ftxui/dom/elements.hpp"
#include "..//inc/ftxui/screen/screen.hpp"

#include "ftxui/component/captured_mouse.hpp"     // for ftxui
#include "ftxui/component/component.hpp"          // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

class ViewFTXuserInterface //: public View
{
public:
    void run();
    void printDirectory();
    void printFiles();

    std::vector<std::vector<std::string>> printDir();
    std::vector<std::vector<std::string>> printAllFiles();

    std::string get_permission_string(fs::perms permission);
    void generateColorTable(ftxui::Table *table);
    void refreshDir(std::vector<std::string> &dirNames);
};
