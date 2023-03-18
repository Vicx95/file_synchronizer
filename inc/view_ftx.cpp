#include "..//inc/view_ftx.hpp"

void ViewFTXuserInterface::run()
{
    int value = 50;
    auto screen = ScreenInteractive::Fullscreen(); // FitComponent();
    bool showTableDir = false;
    bool showTableFile = false;
    bool showAddDir = false;
    bool showRemoveDir = false;
    bool showSetIntVal = false;

    //---------------------------------------------------------------------
    // Buttons on the screen
    //---------------------------------------------------------------------
    // The tree of components. This defines how to navigate using the keyboard.
    auto buttons = Container::Horizontal({Button("Add dir", [&] { /*c.addDirectory(std::cin); */ showAddDir = true; showRemoveDir = false; showSetIntVal = false; }),
                                          Button("Remove Dir", [&] { /*c.removeDirectory(); */ showRemoveDir = true; showTableDir = false; showTableFile = false; showAddDir = false; showSetIntVal = false; }),
                                          Button("Remove File", [&] { /*c.removeFile(); */ }),
                                          Button("Print Dirs", [&] { /*listener->printDirectory();*/ showTableDir = true; showTableFile = false; showRemoveDir=false; showAddDir =false; showSetIntVal = false; }),
                                          Button("Print files", [&] { /*c.printFiles(); */ showTableFile = true; showTableDir = false; showAddDir = false; showSetIntVal = false; }),
                                          Button("Set int-val", [&] { /*c.setIntervalTime(std::cin); */ showSetIntVal = true; showTableDir = false; showTableFile = false; showRemoveDir=false; showAddDir =false; }),
                                          Button("Start sync-up", [&] { listener->startSync(); showTableDir = false; showTableFile = false; showAddDir = false; showSetIntVal = false; }),
                                          Button("Stop sync-up", [&] { listener->stopSync(); }),
                                          Button("Force sync-up", [&] { listener->forceSync(); }),
                                          Button("Read config", [&] { listener->readConfig(); }),
                                          Button("Save config", [&] { listener->saveConfig(); }),
                                          Button("Exit", [&] { screen.Exit(); })

    });

    //---------------------------------------------------------------------
    // Print directory and files
    //---------------------------------------------------------------------
    ftxui::Table tableDirs = printDir();       // listener->printDirectory();
    ftxui::Table tableFiles = printAllFiles(); // listener->printFiles();
    // std::vector<ftxui::Table*> Tables {&tableDirs, &tableFiles };
    generateColorTable(&tableDirs);
    generateColorTable(&tableFiles);

    Element printDirTable = tableDirs.Render();
    Element printFilesTable = tableFiles.Render();
    //----------------------------------------------------------------------
    // Print logo-graph
    //----------------------------------------------------------------------
    Element document = graph([](int x, int y) {
        std::vector<int> result(50, 0);
        for (int i = 0; i < x; ++i)
        {
            result.push_back(((3 * i) / 2) % y);
        }
        return result;
    });

    // document |= color(Color::Green3);
    document |= bgcolor(Color::DarkBlue);
    document |= border;

    //---------------------------------------------------------------------
    // Remove directory
    //---------------------------------------------------------------------
    std::vector<std::string> dirNames;
    int dirSelected = 0;
    // bool enterRemoveDir = false;
    auto menu_option = MenuOption();
    refreshDir(dirNames);
    menu_option.on_enter = [&] {
        // enterRemoveDir = true;
        std::stringstream rmStream(*(dirNames.begin() + dirSelected));
        listener->removeDirectory(rmStream);
        dirNames.erase(dirNames.begin() + dirSelected);

        refreshDir(dirNames);
    };
    Component inputRemove = Menu(&dirNames, &dirSelected, menu_option);

    //---------------------------------------------------------------------

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
    // Set int-val
    //---------------------------------------------------------------------
    unsigned int interval = 5000;
    std::stringstream setInterval(interval);

    // The tree of components. This defines how to navigate using the keyboard.
    auto IntervalButtons = Container::Horizontal({
        Button(
            "Decrease", [&] { interval > 0 ? interval -= 1000 : interval = 0; }, ButtonOption::Animated(Color::Red)),
        Button("Reset", [&] { interval = 5000; }),
        Button(
            "Increase", [&] { interval < 60000 ? interval += 1000 : interval = 60000; }, ButtonOption::Animated(Color::Green)),
        Button("Set", [&] { listener->setIntervalTime(setInterval); }) | flex,
    });

    //---------------------------------------------------------------------

    Component container = Container::Horizontal({
        buttons,
        // input_first_name,
        inputAdd,
        inputRemove,
        IntervalButtons,
    });

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
                listener->addDirectory(InputDirName);
                refreshDir(dirNames);
            }
            tableDirs = printDir(); // listener->printDirectory();
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
                   document,
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
                                       gauge(interval * 0.0000165f),
                                       separator(),
                                       IntervalButtons->Render(),
                                   }) | hcenter
                                 : emptyElement(),
               }) |
               border;
    });

    screen.Loop(component);
}

std::vector<std::vector<std::string>> ViewFTXuserInterface::printAllFiles()
{
    std::set<fs::path> sorted_by_name;

    std::vector<std::vector<std::string>> rows;
    rows.push_back({"File Name", "Modification Time", "Size", "Permissions"});

    for (auto const &entry : fs::recursive_directory_iterator(mainDirectoryPath)) // directory_iterator
    {
        if (entry.is_regular_file())
        {
            auto file = entry.path();
            auto file_time = fs::last_write_time(file);
            auto file_size = fs::file_size(file);
            auto file_perms = fs::status(file).permissions();
            std::time_t cftime = decltype(file_time)::clock::to_time_t(file_time);
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

void ViewFTXuserInterface::refreshDir(std::vector<std::string> &dirNames)
{
    dirNames.clear();
    for (auto const &entry : fs::directory_iterator(mainDirectoryPath)) // directory_iterator
    {
        dirNames.push_back(entry.path().filename().string());
    }
    std::sort(dirNames.begin(), dirNames.end());
}

void ViewFTXuserInterface::printFiles()
{
}

void ViewFTXuserInterface::printDirectory()
{
}