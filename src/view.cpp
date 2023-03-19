#include "..//inc/view.hpp"

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
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


void ViewConsoleUserInterface::run()
{
    
        printMenu();
        printOptions();

}

void ViewFTXuserInterface::run()
{
    float value = 50.0f;
    auto screen = ScreenInteractive::Fullscreen(); //FitComponent();
    bool showAddDir = false;
    bool showRemoveDir = false;
    bool showRemoveFile = false;
    bool showTableDir = false;
    bool showTableFile = false; 
    bool showSetIntVal = false;
    bool showStartSync = false;
    bool showStopSync = false;
    std::deque<bool*> showButtons {&showAddDir, &showRemoveDir, &showRemoveFile, &showTableDir, &showTableFile, &showSetIntVal, &showStartSync, &showStopSync };
    std::vector<std::string> dirNamesX; 
    //unsigned int shift = 0;
    std::condition_variable cv;
    std::mutex cv_mutex;
    std::atomic<bool> refresh_ui_continue = true;

    auto buttons = createButtons(showButtons, cv, refresh_ui_continue, dirNamesX, screen);

    Component container = Container::Horizontal({
      buttons,
      //input_first_name,
    //   inputAdd,
    //   inputRemove,
    //   IntervalButtons,
    //   layoutRemoveFile,
    //   gauge_component,
    });


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
              /// showTableDir ?  printDirTable | hcenter : emptyElement(),
             //  showTableFile ? printFilesTable | hcenter : emptyElement(),
            //    showAddDir ?
            //    vbox({ 
            //    separator(),
            //    hbox(text(" Input directory name: "), inputAdd->Render()),
            //    hflow(render_command()) | flex_grow,
            //    }) : emptyElement(),
            //    showRemoveDir ?  inputRemove->Render() | vscroll_indicator | frame |   size(HEIGHT, EQUAL, 3) | flex : emptyElement(),
            //    showSetIntVal ?  vbox({ text("Interval time value [milliseconds]: " + std::to_string(interval)), separator(), gauge(interval * 0.0000165f), separator(), IntervalButtons->Render(), }) | hcenter : emptyElement(),
            //    showRemoveFile ? layoutRemoveFile->Render() : emptyElement(),
            //    showStartSync ?  gauge_component->Render() : emptyElement(),
              // showStopSync ?  refresh_ui_continue = false :  emptyElement(),

           }) |
           border;
      });

  screen.Loop(component);
}

ftxui::Component ViewFTXuserInterface::createButtons(std::deque<bool*> &showButtons, std::condition_variable& cv, std::atomic<bool>& refresh_ui_continue, std::vector<std::string>& dirNamesX, ftxui::ScreenInteractive& screen)
{
// The tree of components. This defines how to navigate using the keyboard.
  auto buttons = Container::Horizontal({
      Button("Add dir", [&] { hideMenuButtons(showButtons); *showButtons[0] = true; }),
      Button("Remove Dir", [&] { hideMenuButtons(showButtons); *showButtons[1] = true; }),
      Button("Remove File", [&] { hideMenuButtons(showButtons); *showButtons[2] = true; refreshDir(dirNamesX);}),
      Button("Print Dirs", [&] { hideMenuButtons(showButtons); *showButtons[3] = true; }),
      Button("Print files", [&] { hideMenuButtons(showButtons); *showButtons[4] = true; }),
      Button("Set int-val", [&] { hideMenuButtons(showButtons); *showButtons[5] = true; }),
      Button("Start sync-up", [&] { hideMenuButtons(showButtons); *showButtons[6] = true; 
        refresh_ui_continue = true;
        cv.notify_one();

      }),
      Button("Stop sync-up", [&] { *showButtons[7] = true; 
        {
         refresh_ui_continue = false;
        }
         cv.notify_one();
        //shift = 0; 
      }),
      Button("Force sync-up", [&] { }),
      Button("Read config", [&] { }),
      Button("Save config", [&] { }),
      Button("Exit", [&] {screen.Exit();})

  });

  return buttons;
}

void ViewFTXuserInterface::hideMenuButtons(std::deque<bool*> &showButtons)
{     
    for(auto button : showButtons)
    {
        *button = false;
    }
}

void ViewFTXuserInterface::refreshDir(std::vector<std::string> &dirNames)
{
    std::vector<std::string> vec = dirNames;
    /*
    dirNames.clear();
    for (auto const &entry : fs::directory_iterator(m_mainDirectoryPath))  //directory_iterator
    {
        dirNames.push_back(entry.path().filename().string());
    }
    std::sort(dirNames.begin(), dirNames.end());
    */
}
