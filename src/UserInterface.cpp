#include "..//inc/UserInterface.hpp"
#include "..//inc/view.hpp"
#include <thread>

#include "..//inc/ftxui/dom/elements.hpp"
#include "..//inc/ftxui/screen/screen.hpp"



#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
using namespace ftxui;


UserInterface::UserInterface()
{
   // mainDirectoryPath->setMainDirectoryPath();
 //  if(m_view->getListener())
       //m_listener = m_view->getListener();

}
/*
ConsoleUserInterface::ConsoleUserInterface(View *view, i_ViewListener *listener)
    : m_view(view), m_listener(listener)
{
   // mainDirectoryPath->setMainDirectoryPath();

}
*/
/*
void UserInterface::setListener(i_ViewListener *listener)
{
    this->m_listener = listener;
}

i_ViewListener* UserInterface::getListener()
{
    return m_listener;
}
*/
void ConsoleUserInterface::run(){
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
            m_listener->addDirectory(std::cin);
            break;
        case Action::RemoveDir:
            m_listener->removeDirectory();
            break;
        case Action::RemoveFile:
            m_listener->removeFile();
            break;
        case Action::PrintDir:
            m_listener->printDirectory();
            break;
        case Action::PrintFiles:
            m_listener->printFiles();
            break;
        case Action::SetIntervalTime:
            m_listener->setIntervalTime(std::cin);
            break;
        case Action::StartSync:
            m_listener->startSync();
            break;
        case Action::StopSync:
            m_listener->stopSync();
            break;
        case Action::ForceSync:
            m_listener->forceSync();
            break;
        case Action::readConfig:
            m_listener->readConfig();
            break;
        case Action::saveConfig:
            m_listener->saveConfig();
            break;
        case Action::Exit:
            std::cout << "one";
            m_isExitRequested = m_listener->exit();
            break;
        default:
            std::cout << "Incorrect action selected! Please try again...\n";
            waitForButton();
            break;
            
        }
        
    }

}

void ConsoleUserInterface::printFiles(){}
void ConsoleUserInterface::printDirectory(){

    fs::current_path(m_view->getMainDirectoryPath());
    std::cout << "Print current path: " << m_view->getMainDirectoryPath() << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(m_view->getMainDirectoryPath()))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(m_view->getMainDirectoryPath()))
        std::cout << "Folder is empty...\n"; 
    waitForButton();
    
}
/*
void UserInterface::setMainDirectoryPath(const fs::path &path)
{
    mainDirectoryPath = path;
}
*/

//void ConsoleUserInterface::run(){


void FTXuserInterface::run(){
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

void FTXuserInterface::printFiles(){}
void FTXuserInterface::printOptions(){}
void FTXuserInterface::printDirectory(){}
void FTXuserInterface::printMenu(){}

void ConsoleUserInterface::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void ConsoleUserInterface::printOptions()
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

void UserInterface::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}