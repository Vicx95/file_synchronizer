#include "..//inc/view.hpp"
#include "..//inc/model.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <set>
#include <thread>


#include "..//inc/ftxui/dom/elements.hpp"
#include "..//inc/ftxui/screen/screen.hpp"
#include <ftxui/dom/table.hpp>      // for Table, TableSelection

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
using namespace ftxui;

namespace fs = std::filesystem;


void View::setListener(i_ViewListener *listener)
{
    this->listener = listener;
}

void ViewConsoleUserInterface::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void ViewConsoleUserInterface::printOptions()
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

void ViewConsoleUserInterface::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}

std::string ViewFTXuserInterface::get_permission_string(fs::perms permission) {
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

std::vector<std::vector<std::string>> ViewFTXuserInterface::printDir()
{
    fs::current_path(mainDirectoryPath);

    std::vector<std::vector<std::string>> rows;
    rows.push_back({ "Directory Name", "Modification Time", "Size", "Permissions" });
  

    for (auto const &entry : fs::directory_iterator(mainDirectoryPath))  //directory_iterator
    {        
        auto file = entry.path();
        auto file_time = fs::last_write_time(file);
        //auto file_size = 0;//fs::file_size(file);
        auto file_perms = fs::status(file).permissions();
        std::time_t cftime = decltype(file_time)::clock::to_time_t(file_time);
        std::string modification_time = std::ctime(&cftime);

        rows.push_back({ file.filename().string(), modification_time, std::to_string(0) + " Bytes", get_permission_string(file_perms) }); 
    }
    std::sort(rows.begin()+1, rows.end());

    return rows;
 
}

void ViewConsoleUserInterface::printDirectory()
{

    int counter = 0;
    fs::current_path(mainDirectoryPath);

    for (auto const &dirEntry : fs::directory_iterator(mainDirectoryPath))
    {
        std::cout << dirEntry.path() << std::endl;
        counter++;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton(); 
}

bool ViewConsoleUserInterface::validateForPrinting(std::string name)
{
    if (name == "all")
    {
        return true;
    }
    if (!fs::exists(fs::current_path() / name))
    {
        std::cout << "Not exist file or directory\n";
        waitForButton();
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> ViewFTXuserInterface::printAllFiles()
{
    std::set<fs::path> sorted_by_name;

    std::vector<std::vector<std::string>> rows;
    rows.push_back({ "File Name", "Modification Time", "Size", "Permissions" });


    for (auto const &entry : fs::recursive_directory_iterator(mainDirectoryPath))  //directory_iterator
    {
        if (entry.is_regular_file()) {
            auto file = entry.path();
            auto file_time = fs::last_write_time(file);
            auto file_size = fs::file_size(file);
            auto file_perms = fs::status(file).permissions();
            std::time_t cftime = decltype(file_time)::clock::to_time_t(file_time);
            std::string modification_time = std::ctime(&cftime);

            rows.push_back({ file.filename().string(), modification_time, std::to_string(file_size) + "KB", get_permission_string(file_perms) });  //std::to_string(file_perms)
        }
    }

    return rows;
}

void ViewConsoleUserInterface::printFiles()
{
    std::set<fs::path> sorted_by_name;

    std::cout << "Give folder name or choose 'all' to print files: \n";
    std::string dirName;
    std::cin.clear();
    std::cin >> dirName;

    if (validateForPrinting(dirName))
    {
        if (dirName == "all")
        {
            dirName = mainDirectoryPath;
        }
        for (auto const &dirEntry : fs::recursive_directory_iterator(dirName))
        {
            sorted_by_name.insert(dirEntry.path());
        }

        for (auto const &fileName : sorted_by_name)
        {
            if (fs::is_directory(fileName))
            {
                std::cout << "-------------------\n";
                std::cout << "Directory: " << fileName.filename() << ":\n";
            }
            if (fs::is_regular_file(fileName))
            {
                std::cout << fileName.filename() << "\n";
            }
        }

        if (fs::is_empty(mainDirectoryPath / dirName))
            std::cout << "Folder is empty...";

        std::cout << "\n";
        waitForButton();
    }
}

void ViewFTXuserInterface::generateColorTable(ftxui::Table* table)
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
    for (auto const &entry : fs::directory_iterator(mainDirectoryPath))  //directory_iterator
    {
        dirNames.push_back(entry.path().filename().string());
    }
    std::sort(dirNames.begin(), dirNames.end());
}

void ViewConsoleUserInterface::run()
{
    
    std::string inputKey;
    std::regex keyRegex("([0-9]{1})");

    while (!m_isExitRequested)
    {
        printMenu();
        printOptions();

        std::cin.clear();
        std::cin >> inputKey;

        if (!std::regex_search(inputKey, keyRegex))
        {
            std::cout << "Incorrect action selected! Please try again...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            //usleep(1000000); // would be nice to replace it with std::this_thread::sleep_for()
            continue;
        }

        switch (static_cast<Action>(std::stoul(inputKey)))
        {
        case Action::AddDir:
            listener->addDirectory(std::cin);
            break;
        case Action::RemoveDir:
            listener->removeDirectory(std::cin);
            break;
        case Action::RemoveFile:
            listener->removeFile();
            break;
        case Action::PrintDir:
            listener->printDirectory();
            break;
        case Action::PrintFiles:
            listener->printFiles();
            break;
        case Action::SetIntervalTime:
            listener->setIntervalTime(std::cin);
            break;
        case Action::StartSync:
            listener->startSync();
            break;
        case Action::StopSync:
            listener->stopSync();
            break;
        case Action::ForceSync:
            listener->forceSync();
            break;
        case Action::readConfig:
            listener->readConfig();
            break;
        case Action::saveConfig:
            listener->saveConfig();
            break;
        case Action::Exit:
            m_isExitRequested = listener->exit();
            break;
        default:
            std::cout << "Incorrect action selected! Please try again...\n";
            waitForButton();
            break;
        }
    }
}


void ViewFTXuserInterface::run()
{
    int value = 50;
    auto screen = ScreenInteractive::Fullscreen(); //FitComponent();
    bool showTableDir = false;
    bool showTableFile = false; 
    bool showAddDir = false; 
    bool showRemoveDir = false;
    bool showSetIntVal = false;



//---------------------------------------------------------------------
//Buttons on the screen
//---------------------------------------------------------------------
  // The tree of components. This defines how to navigate using the keyboard.
  auto buttons = Container::Horizontal({
      Button("Add dir", [&] { /*c.addDirectory(std::cin); */ showAddDir = true; showRemoveDir = false; showSetIntVal = false;}),
      Button("Remove Dir", [&] { /*c.removeDirectory(); */ showRemoveDir = true; showTableDir = false; showTableFile = false; showAddDir = false; showSetIntVal = false;}),
      Button("Remove File", [&] { /*c.removeFile(); */}),
      Button("Print Dirs", [&] { /*listener->printDirectory();*/ showTableDir = true; showTableFile = false; showRemoveDir=false; showAddDir =false; showSetIntVal = false;}),
      Button("Print files", [&] { /*c.printFiles(); */ showTableFile = true; showTableDir = false; showAddDir = false; showSetIntVal = false;}),
      Button("Set int-val", [&] { /*c.setIntervalTime(std::cin); */ showSetIntVal = true; showTableDir = false; showTableFile = false; showRemoveDir=false; showAddDir =false;}),
      Button("Start sync-up", [&] { listener->startSync(); showTableDir = false; showTableFile = false; showAddDir = false; showSetIntVal = false; }),
      Button("Stop sync-up", [&] { listener->stopSync(); }),
      Button("Force sync-up", [&] { listener->forceSync();}),
      Button("Read config", [&] { listener->readConfig(); }),
      Button("Save config", [&] { listener->saveConfig(); }),
      Button("Exit", [&] {screen.Exit();})

  });


//---------------------------------------------------------------------
// Print directory and files
//---------------------------------------------------------------------
  ftxui::Table tableDirs = printDir();//listener->printDirectory();
  ftxui::Table tableFiles = printAllFiles();//listener->printFiles();
//std::vector<ftxui::Table*> Tables {&tableDirs, &tableFiles };
    generateColorTable(&tableDirs);
    generateColorTable(&tableFiles);


  Element printDirTable = tableDirs.Render();
  Element printFilesTable = tableFiles.Render();
//----------------------------------------------------------------------
// Print logo-graph
//----------------------------------------------------------------------
  Element document = graph([](int x, int y) {
    std::vector<int> result(50, 0);
    for (int i=0; i < x; ++i) {
      result.push_back(((3 * i) / 2) % y);
    }
    return result;
  });

 // document |= color(Color::Green3);
  document |= bgcolor(Color::DarkBlue);
  document |= border;

//---------------------------------------------------------------------
//Remove directory
//---------------------------------------------------------------------
std::vector<std::string> dirNames; 
int dirSelected = 0;
//bool enterRemoveDir = false;
auto menu_option = MenuOption();
refreshDir(dirNames);
menu_option.on_enter = [&] {
    //enterRemoveDir = true;
    std::stringstream rmStream(*(dirNames.begin()+dirSelected));
    listener->removeDirectory(rmStream);
    dirNames.erase(dirNames.begin()+dirSelected);

    refreshDir(dirNames);
};
Component inputRemove = Menu(&dirNames, &dirSelected, menu_option);

//---------------------------------------------------------------------


//---------------------------------------------------------------------
//Add directory
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
      Button("Decrease", [&] { interval > 0 ? interval-=1000 : interval = 0;}, ButtonOption::Animated(Color::Red)),
      Button("Reset", [&] { interval = 5000; }), 
      Button("Increase", [&] { interval < 60000 ? interval+=1000 : interval = 60000; }, ButtonOption::Animated(Color::Green)),
      Button("Set", [&] { listener->setIntervalTime(setInterval);}) | flex ,
  });

//---------------------------------------------------------------------


Component container = Container::Horizontal({
      buttons,
      //input_first_name,
      inputAdd,
      inputRemove,
      IntervalButtons,
  });

 auto render_command = [&] {
    Elements line;
    // Input
    for (auto& it : input_entries) {
      line.push_back(text(" " + it) | color(Color::RedLight));
      //Add directory 
       std::stringstream InputDirName(input_entries.back());
       if(enterInputDir){
           enterInputDir = false;
           listener->addDirectory(InputDirName);
           refreshDir(dirNames);
       }
        tableDirs = printDir(); //listener->printDirectory();
        generateColorTable(&tableDirs);
        printDirTable = tableDirs.Render();
    }
    
    return line;
  };


//---------------------------------------------------------------------
// Render screen
//---------------------------------------------------------------------
  // Modify the way to render them on screen:
  auto component = Renderer( container, [&] {
    return vbox({
               text("### FILE SYNCHRONIZER ###") | border | bold | color(Color::DarkGreen) | hcenter,
               separator(),
               gauge(value * 0.01f) | color(Color::RedLight),
               separator(),
               buttons->Render() | color(Color::Orange1),
               separator(),
               document,
               showTableDir ?  printDirTable | hcenter : emptyElement(),
               showTableFile ? printFilesTable | hcenter : emptyElement(),
               showAddDir ?
               vbox({ 
               separator(),
               hbox(text(" Input directory name: "), inputAdd->Render()),
               hflow(render_command()) | flex_grow,
               }) : emptyElement(),
               showRemoveDir ?  inputRemove->Render() | vscroll_indicator | frame |   size(HEIGHT, EQUAL, 3) | flex : emptyElement(),
               showSetIntVal ?  vbox({ text("Interval time value [milliseconds]: " + std::to_string(interval)), separator(), gauge(interval * 0.0000165f), separator(), IntervalButtons->Render(), }) | hcenter : emptyElement(),
           }) |
           border;
  });


  screen.Loop(component);
}

void View::setMainDirectoryPath(const fs::path &path)
{
    mainDirectoryPath = path;
}

void ViewFTXuserInterface::printFiles()
{

}

void ViewFTXuserInterface::printDirectory()
{

}
