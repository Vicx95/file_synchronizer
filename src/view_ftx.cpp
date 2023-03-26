#include "inc/view_ftx.hpp"
#include "inc/view.hpp"

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
    unsigned int shift = 0;
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
        m_model->removeDirectory(rmStream.str()); // listener->removeDirectory(rmStream); ->  controller->removeDirectory() TODO:
        refreshDir(dirNames, path);
    };
    Component inputRemove = Menu(&dirNames, &dirSelected, menu_option);

    //---------------------------------------------------------------------
    // Remove file
    //---------------------------------------------------------------------
    std::vector<std::string> dirsX;
    std::vector<std::vector<std::string>> filesX;
    std::deque<bool> dirMenuShow;

    for (auto dir : fs::directory_iterator(path))
    {
        if (!dir.is_regular_file())
        {
            dirsX.push_back(dir.path().filename());
            dirMenuShow.push_back(false);
        }
    }
    std::sort(dirsX.begin(), dirsX.end());

    for (auto dir : dirsX)
    {
        std::vector<std::string> filesInDir;
        for (auto const &fileEntry : fs::recursive_directory_iterator(path / dir))
        {
            if (fileEntry.is_regular_file())
            {
                filesInDir.push_back((fileEntry.path().filename()));
            }
        }
        std::sort(filesInDir.begin(), filesInDir.end());
        filesX.push_back(filesInDir);
    }

    auto checkAndMenuboxes = Container::Vertical({});
    auto removefileMenuOption = MenuOption();
    std::vector<int>::size_type it = 0;
    int selectedMenuFile = 0;

    for (auto dir : dirsX)
    {
        checkAndMenuboxes->Add(
            Checkbox(dir, &dirMenuShow[it]));

        auto current_it = it;
        removefileMenuOption.on_enter = [&, current_it] {
            if (filesX[current_it].size() > (std::vector<std::string>::size_type)selectedMenuFile)
            { // check if selected file exists
                auto it_file = filesX[current_it].begin() + selectedMenuFile;
                std::stringstream rmStream(*it_file); // create stream for file name
                // listener->removeFile(rmStream);
                m_model->removeFile(rmStream);
                refreshFile(dirsX, filesX, path);
            }
        };

        checkAndMenuboxes->Add(
            Menu(&filesX[it], &selectedMenuFile, removefileMenuOption) | border | Maybe(&dirMenuShow[it]));
        it++;
    }

    auto layoutRemoveFile = Container::Vertical({
        checkAndMenuboxes,
    });
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
    // Start sync
    //---------------------------------------------------------------------
    auto render_gauge = [&shift, &interval](unsigned int delta) {
        float progress = (float)((shift + delta) % (interval/1000)) / (float)(interval/1000); 
        delta = 0;
        return hbox({
            text("Synchronization: "),

            text(std::to_string(int(progress * 100)) + "% ") |
                size(WIDTH, EQUAL, 5),
            gauge(progress),
        });
    };

    auto render_gauge2 = [&shift, &interval](unsigned int delta) {
        float progress = (float)((shift + delta) % (interval/1000)) / (float)(interval/1000);
        return hbox({
            text("Time interval:   "),
            text(std::to_string(int(progress * (float)(interval / 1000))) + "s ") |
                size(WIDTH, EQUAL, 5),
            gauge(progress),
        });
    };

    auto gauge_component = Renderer([render_gauge, render_gauge2] {
        return vbox({
            render_gauge(0) | color(Color::MagentaLight),
            render_gauge2(0) | color(Color::YellowLight),
    });
    });
    //---------------------------------------------------------------------
    // Generate components to render
    //---------------------------------------------------------------------
    Component container = Container::Horizontal({
        buttons,
        inputAdd,
        inputRemove,
        IntervalButtons,
        layoutRemoveFile,
        gauge_component,
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
                m_model->addDirectory(input_entries.back()); // listener->addDirectory(InputDirName);
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
                   showRemoveFile ? layoutRemoveFile->Render() : emptyElement(),
                    showStartSync ?  gauge_component->Render() : emptyElement(),
                   // showStopSync ?  refresh_ui_continue = false :  emptyElement(),

               }) |
               border;
    });

    std::jthread refresh_ui(&ViewFTXuserInterface::synchronizationFTX, this, std::ref(cv_mutex), std::ref(cv), std::ref(screen), std::ref(shift), std::ref(interval), std::ref(refresh_ui_continue));

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
                                          Button("Force sync-up", [&] { m_model->forceSync(); }), Button("Read config", [&] {}), Button("Save config", [&] {}), Button("Exit", [&] { screen.Exit(); })

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

void ViewFTXuserInterface::refreshFile(std::vector<std::string> &dirNames, std::vector<std::vector<std::string>> &filesNames, const fs::path &path)
{
    filesNames.clear();
    for (auto dir : dirNames)
    {
        std::vector<std::string> filesInDir;
        for (auto const &fileEntry : fs::recursive_directory_iterator(path / dir))
        {
            if (fileEntry.is_regular_file())
            {
                filesInDir.push_back((fileEntry.path().filename()));
            }
        }
        std::sort(filesInDir.begin(), filesInDir.end());
        filesNames.push_back(filesInDir);
    }
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

void ViewFTXuserInterface::synchronizationFTX(std::mutex& cv_mutex, std::condition_variable& cv, ftxui::ScreenInteractive& screen, unsigned int& shift, unsigned int& interval, std::atomic<bool>& refresh_ui_continue)
{
    while (true) {
        std::unique_lock<std::mutex> lock(cv_mutex);
        if(shift % (interval/1000) == 0){
            m_model->forceSync();
            shift = 1;
        }
        cv.wait(lock, [&] { return refresh_ui_continue.load(); });
        if (!refresh_ui_continue.load()) {
            // Exit the loop if refresh_ui_continue is false
            break;
        }
        std::this_thread::sleep_for(1s);
        screen.Post([&] { shift++; });

        screen.Post(Event::Custom);
        lock.unlock();
    }
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

std::string ViewFTXuserInterface::getTypeUI()
{
    return "FTX";
}