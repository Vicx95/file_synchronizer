#include "..//inc/view.hpp"
#include "..//inc/view_console.hpp"
#include "..//inc/view_ftx.hpp"

#include <sstream>

#include "ftxui/component/captured_mouse.hpp"     // for ftxui
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for separator, gauge, text, Element, operator|, vbox, border
using namespace ftxui;
using namespace std::chrono_literals;

void View::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void View::printOptions()
{
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Start sync-up  \n"
              << "8. Stop sync-up  \n"
              << "9. Force sync-up  \n"
              << "10. Read config  \n"
              << "11. Save config  \n";
}

void View::printDirectory(const fs::path &path)
{
    fs::current_path(path);
    std::cout << "Print current path: " << path << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(path))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(path))
        std::cout << "Folder is empty...\n";
}

bool View::validateForPrinting(std::string name)
{
    if (name == "all")
    {
        return true;
    }
    if (!fs::exists(fs::current_path() / name))
    {
        std::cout << "File or directory doesn't exist\n";
        return false;
    }
    return true;
}

void View::printMessage(Message m)
{
    switch (m)
    {
    case Message::GiveFolder:
        std::cout << "Give folder name to add: \n";
        break;
    case Message::DirExists:
        std::cout << "Dir already exist...\n";
        break;
    case Message::Incorrect:
        std::cout << "Incorrect action selected! Please try again...\n";
        break;
    case Message::Interval:
        std::cout << "Interval time value [milliseconds]: \n";
        break;
    case Message::Remove:
        std::cout << "Give folder name to remove: \n";
        break;
    case Message::RemoveFile:
        std::cout << "Give folder name with files to delete: \n";
        break;
    case Message::PrintFiles:
        std::cout << "Give folder name or choose 'all' to print files: \n";
        break;
    case Message::FolderEmpty:
        std::cout << "Folder is empty...";
        break;
    default:
        std::cout << "Message string not found";
        break;
    }
}

void View::printFiles(const std::set<fs::path> &fileDirList)
{
    for (auto const &fileName : fileDirList)
    {
        if (fs::is_directory(fileName))
        {
            std::cout << "-------------------\n";
            std::cout << "Directory: " << fileName.filename() << ":\n";
        }
        else if (fs::is_regular_file(fileName))
        {
            std::cout << fileName.filename() << "\n";
        }
    }
}

void ViewConsoleUserInterface::run(const fs::path &path)
{
    fs::path spath = path;
    printMenu();
    printOptions();
}

void ViewFTXuserInterface::setModel(i_Model *ptr)
{
    if (ptr)
    {
        m_model = ptr;
    }
}

void ViewFTXuserInterface::run(const fs::path &path)
{
    float value = 50.0f;
    auto screen = ScreenInteractive::Fullscreen(); // FitComponent();
    bool showAddDir = false;
    bool showRemoveDir = false;
    bool showRemoveFile = false;
    bool showTableDir = false;
    bool showTableFile = false;
    bool showSetIntVal = false;
    bool showStartSync = false;
    bool showStopSync = false;
    std::deque<bool *> showButtons{&showAddDir, &showRemoveDir, &showRemoveFile, &showTableDir, &showTableFile, &showSetIntVal, &showStartSync, &showStopSync};
    std::vector<std::string> dirNamesX;
    // unsigned int shift = 0;
    std::condition_variable cv;
    std::mutex cv_mutex;
    std::atomic<bool> refresh_ui_continue = true;

    auto buttons = createButtons(showButtons, cv, refresh_ui_continue, dirNamesX, screen, path);

    //----------------------------------------------------------------------
    // Print logo-graph
    //----------------------------------------------------------------------
    ftxui::Element logoGraph = printLogoGraph();
    logoGraph |= bgcolor(Color::DarkBlue);
    logoGraph |= border;

    //---------------------------------------------------------------------
    // Print directory and files
    //---------------------------------------------------------------------
    ftxui::Table tableDirs = printDir(path);
    ftxui::Table tableFiles = printAllFiles(path);
    generateColorTable(&tableDirs);
    generateColorTable(&tableFiles);

    Element printDirTable = tableDirs.Render();
    Element printFilesTable = tableFiles.Render();

    //---------------------------------------------------------------------
    // Add directory
    //---------------------------------------------------------------------
    std::vector<std::string> input_entries;
    auto input_option = InputOption();
    std::string input_add_content;
    bool enterInputDir = false;
    input_option.on_enter = [&] {
        input_entries.push_back(input_add_content);
        input_add_content = "";
        enterInputDir = true;
    };
    Component inputAdd = Input(&input_add_content, "input directory", input_option);

    //---------------------------------------------------------------------
    // Remove directory
    //---------------------------------------------------------------------
    std::vector<std::string> dirNames;
    int dirSelected = 0;
    auto menu_option = MenuOption();
    refreshDir(dirNames, path);
    menu_option.on_enter = [&] {
        std::stringstream rmStream(*(dirNames.begin() + dirSelected));
        // listener->removeDirectory(rmStream); ->  controller->removeDirectory() TODO:
        refreshDir(dirNames, path);
    };
    Component inputRemove = Menu(&dirNames, &dirSelected, menu_option);

    //---------------------------------------------------------------------
    // Set int-val
    //---------------------------------------------------------------------
    unsigned int interval = 5000;
    std::stringstream setInterval(std::to_string(interval));

    // The tree of components. This defines how to navigate using the keyboard.
    auto IntervalButtons = Container::Horizontal({
        Button(
            "Decrease", [&] { interval > 0 ? interval -= 1000 : interval = 0; }, ButtonOption::Animated(Color::Red)),
        Button("Reset", [&] { interval = 5000; }),
        Button(
            "Increase", [&] { interval < 60000 ? interval += 1000 : interval = 60000; }, ButtonOption::Animated(Color::Green)),
        Button("Set", [&] { m_model->setIntervalTime(2000ms); }) | flex,
    });

    //---------------------------------------------------------------------
    // Generate components to render
    //---------------------------------------------------------------------
    Component container = Container::Horizontal({
        buttons,
        // input_first_name,
        inputAdd,
        inputRemove,
        IntervalButtons,
        //   layoutRemoveFile,
        //   gauge_component,
    });

    //---------------------------------------------------------------------
    // Renderer added and removed dirs in print tables
    //---------------------------------------------------------------------
    auto render_command = [&] {
        Elements line;
        // Input
        for (auto &it : input_entries)
        {
            line.push_back(text(" " + it) | color(Color::RedLight));
            // Add directory
            std::stringstream InputDirName(input_entries.back());
            if (enterInputDir)
            {
                enterInputDir = false;
                // listener->addDirectory(InputDirName);
                refreshDir(dirNames, path);
            }
            tableDirs = printDir(path); // listener->printDirectory(); -> ->  TODO:  controller->addDirectory()
            generateColorTable(&tableDirs);
            printDirTable = tableDirs.Render();
        }

        return line;
    };
    //---------------------------------------------------------------------
    // Render screen
    //---------------------------------------------------------------------
    // Modify the way to render them on screen:
    auto component = Renderer(container, [&] {
        return vbox({
                   text("### FILE SYNCHRONIZER ###") | border | bold | color(Color::DarkGreen) | hcenter,
                   separator(),
                   gauge(value * 0.01f) | color(Color::RedLight),
                   separator(),
                   buttons->Render() | color(Color::Orange1),
                   separator(),
                   logoGraph,
                   showTableDir ? printDirTable | hcenter : emptyElement(),
                   showTableFile ? printFilesTable | hcenter : emptyElement(),
                   showAddDir ? vbox({
                                    separator(),
                                    hbox(text(" Input directory name: "), inputAdd->Render()),
                                    hflow(render_command()) | flex_grow,
                                })
                              : emptyElement(),
                   showRemoveDir ? inputRemove->Render() | vscroll_indicator | frame | size(HEIGHT, EQUAL, 3) | flex : emptyElement(),
                   showSetIntVal ? vbox({
                                       text("Interval time value [milliseconds]: " + std::to_string(interval)),
                                       separator(),
                                       gauge((float)interval * 0.0000165f),
                                       separator(),
                                       IntervalButtons->Render(),
                                   }) | hcenter
                                 : emptyElement(),
                   //    showRemoveFile ? layoutRemoveFile->Render() : emptyElement(),
                   //    showStartSync ?  gauge_component->Render() : emptyElement(),
                   // showStopSync ?  refresh_ui_continue = false :  emptyElement(),

               }) |
               border;
    });

    screen.Loop(component);
}

ftxui::Component ViewFTXuserInterface::createButtons(std::deque<bool *> &showButtons, std::condition_variable &cv, std::atomic<bool> &refresh_ui_continue, std::vector<std::string> &dirNamesX, ftxui::ScreenInteractive &screen, const fs::path &path)
{
    // The tree of components. This defines how to navigate using the keyboard.
    auto buttons = Container::Horizontal({Button("Add dir", [&] { hideMenuButtons(showButtons); *showButtons[0] = true; }),
                                          Button("Remove Dir", [&] { hideMenuButtons(showButtons); *showButtons[1] = true; }),
                                          Button("Remove File", [&] { hideMenuButtons(showButtons); *showButtons[2] = true; refreshDir(dirNamesX, path); }),
                                          Button("Print Dirs", [&] { hideMenuButtons(showButtons); *showButtons[3] = true; }),
                                          Button("Print files", [&] { hideMenuButtons(showButtons); *showButtons[4] = true; }),
                                          Button("Set int-val", [&] { hideMenuButtons(showButtons); *showButtons[5] = true; }),
                                          Button("Start sync-up", [&] {
                                              hideMenuButtons(showButtons);
                                              *showButtons[6] = true;
                                              refresh_ui_continue = true;
                                              cv.notify_one();
                                          }),
                                          Button("Stop sync-up", [&] {
                                              *showButtons[7] = true;
                                              {
                                                  refresh_ui_continue = false;
                                              }
                                              cv.notify_one();
                                              // shift = 0;
                                          }),
                                          Button("Force sync-up", [&] {}), Button("Read config", [&] {}), Button("Save config", [&] {}), Button("Exit", [&] { screen.Exit(); })

    });

    return buttons;
}

void ViewFTXuserInterface::hideMenuButtons(std::deque<bool *> &showButtons)
{
    for (auto button : showButtons)
    {
        *button = false;
    }
}

void ViewFTXuserInterface::refreshDir(std::vector<std::string> &dirNames, const fs::path &path)
{
    std::vector<std::string> vec = dirNames;

    dirNames.clear();
    for (auto const &entry : fs::directory_iterator(path)) // directory_iterator
    {
        dirNames.push_back(entry.path().filename().string());
    }
    std::sort(dirNames.begin(), dirNames.end());
}

std::vector<std::vector<std::string>> ViewFTXuserInterface::printDir(const fs::path &path)
{
    fs::current_path(path);

    std::vector<std::vector<std::string>> rows;
    rows.push_back({"Directory Name", "Modification Time", "Size", "Permissions"});

    for (auto const &entry : fs::directory_iterator(path)) // directory_iterator
    {
        auto file = entry.path();
        auto file_time = fs::last_write_time(file);
        // auto file_size = 0;//fs::file_size(file);
        auto file_perms = fs::status(file).permissions();
        // std::time_t cftime = decltype(file_time)::clock::to_time_t(file_time);
        std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fs::file_time_type::clock::time_point(file_time)));

        std::string modification_time = std::ctime(&cftime);

        rows.push_back({file.filename().string(), modification_time, std::to_string(0) + " Bytes", get_permission_string(file_perms)});
    }
    std::sort(rows.begin() + 1, rows.end());

    return rows;
}

std::vector<std::vector<std::string>> ViewFTXuserInterface::printAllFiles(const fs::path &path)
{
    std::set<fs::path> sorted_by_name;

    std::vector<std::vector<std::string>> rows;
    rows.push_back({"File Name", "Modification Time", "Size", "Permissions"});

    for (auto const &entry : fs::recursive_directory_iterator(path)) // directory_iterator
    {
        if (entry.is_regular_file())
        {
            auto file = entry.path();
            auto file_time = fs::last_write_time(file);
            auto file_size = fs::file_size(file);
            auto file_perms = fs::status(file).permissions();
            // std::time_t cftime = decltype(file_time)::clock::to_time_t(file_time);
            std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(fs::file_time_type::clock::time_point(file_time)));

            std::string modification_time = std::ctime(&cftime);

            rows.push_back({file.filename().string(), modification_time, std::to_string(file_size) + "KB", get_permission_string(file_perms)}); // std::to_string(file_perms)
        }
    }

    return rows;
}

void ViewFTXuserInterface::generateColorTable(ftxui::Table *table)
{
    table->SelectAll().Border(LIGHT);

    // Add border around the first column.
    table->SelectColumn(0).Border(LIGHT);

    // Make first row bold with a double border.
    table->SelectRow(0).Decorate(bold);
    table->SelectRow(0).SeparatorVertical(LIGHT);
    table->SelectRow(0).Border(DOUBLE);

    // Align right the "Release date" column.
    table->SelectColumn(2).DecorateCells(align_right);

    // Select row from the second to the last.
    auto content = table->SelectRows(1, -1);
    // Alternate in between 3 colors.
    content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
    content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
    content.DecorateCellsAlternateRow(color(Color::White), 3, 2);
}

ftxui::Element ViewFTXuserInterface::printLogoGraph()
{
    Element document = graph([](int x, int y) {
        std::vector<int> result(50, 0);
        for (int i = 0; i < x; ++i)
        {
            result.push_back(((3 * i) / 2) % y);
        }
        return result;
    });
    return document;
}

std::string ViewFTXuserInterface::get_permission_string(fs::perms permission)
{
    std::stringstream ss;
    ss << ((permission & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
    ss << ((permission & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
    ss << ((permission & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
    ss << ((permission & fs::perms::group_read) != fs::perms::none ? "r" : "-");
    ss << ((permission & fs::perms::group_write) != fs::perms::none ? "w" : "-");
    ss << ((permission & fs::perms::group_exec) != fs::perms::none ? "x" : "-");
    ss << ((permission & fs::perms::others_read) != fs::perms::none ? "r" : "-");
    ss << ((permission & fs::perms::others_write) != fs::perms::none ? "w" : "-");
    ss << ((permission & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
    return ss.str();
}