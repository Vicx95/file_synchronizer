#include "..//inc/view.hpp"
#include "..//inc/model.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <thread>


#include "..//inc/ftxui/dom/elements.hpp"
#include "..//inc/ftxui/screen/screen.hpp"



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

void View::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}

void View::printDirectory()
{

    fs::current_path(mainDirectoryPath);
    std::cout << "Print current path: " << mainDirectoryPath << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(mainDirectoryPath))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(mainDirectoryPath))
        std::cout << "Folder is empty...\n";
    waitForButton();
}

bool View::validateForPrinting(std::string name)
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

void View::printFiles()
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

void View::run()
{
    /*
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
            listener->removeDirectory();
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
*/
   int value = 50;
    auto screen = ScreenInteractive::FitComponent();


  // The tree of components. This defines how to navigate using the keyboard.
  auto buttons = Container::Horizontal({
      Button("Add dir", [&] { /*c.addDirectory(std::cin); */}),
      Button("Remove Dir", [&] { /*c.removeDirectory(); */}),
      Button("Remove File", [&] { /*c.removeFile(); */}),
      Button("Print Dirs", [&] { /*c.printDirectory(); */}),
      Button("Print files", [&] { /*c.printFiles(); */}),
      Button("Set int-val", [&] { /*c.setIntervalTime(std::cin); */}),
      Button("Start sync-up", [&] { /*c.startSync(); */}),
      Button("Stop sync-up", [&] { /*c.stopSync(); */}),
      Button("Force sync-up", [&] { /*c.forceSync(); */}),
      Button("Read config", [&] { /*c.readConfig(); */}),
      Button("Save config", [&] { /*c.saveConfig();*/ }),
      Button("Exit", [&] {screen.Exit();})
  });


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

  // Modify the way to render them on screen:
  auto component = Renderer(buttons, [&] {
    return vbox({
               text("### FILE SYNCHRONIZER ###") | border | bold | color(Color::DarkGreen) | hcenter,
               separator(),
               gauge(value * 0.01f) | color(Color::RedLight),
               separator(),
               buttons->Render() | color(Color::Orange1),

               separator(),
               document,
           }) |
           border;
  });


  screen.Loop(component);

}

void View::setMainDirectoryPath(const fs::path &path)
{
    mainDirectoryPath = path;
}