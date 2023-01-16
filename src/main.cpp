#include "..//inc/FileSynchronizer.hpp"
#include "..//inc/Scanner.hpp"
#include "..//inc/controller.hpp"
#include "..//inc/model.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/UserInterface.hpp"
#include "..//inc/view.hpp"
#include "..//inc/Serializer.hpp"

#include <chrono> // chrono::system_clock
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>


#include "..//inc/ftxui/dom/elements.hpp"
#include "..//inc/ftxui/screen/screen.hpp"



#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
int main()
{
    
    Timer tm;
    View v;
    //UserInterface *ui;
    Scanner scanner;
    FileSynchronizer sync;

    Model m(&tm, &sync, &scanner);
    Controller c(&v, &m);
    v.setListener(&c);
    v.run();
    

   using namespace ftxui;
 
  // Define the document
  
  Element document2 =
    hbox({
      text("Dir")   | border,
      text("Filesynchronizer") | border | flex,
      text("Size")  | border,
    });
  
  //auto screen2 = Screen::Create(
  //  Dimension::Full(),       // Width
  //  Dimension::Fit(document2) // Height
  //);
  //Render(screen2, document2);
  //screen2.Print();
/*
 int value = 50;
auto screen = ScreenInteractive::FitComponent();

 
  // The tree of components. This defines how to navigate using the keyboard.
  auto buttons = Container::Horizontal({
      Button("Add dir", [&] { c.addDirectory(std::cin); }),
      Button("Remove Dir", [&] { c.removeDirectory(); }),
      Button("Remove File", [&] { c.removeFile(); }),
      Button("Print Dirs", [&] { c.printDirectory(); }),
      Button("Print files", [&] { c.printFiles(); }),
      Button("Set int-val", [&] { c.setIntervalTime(std::cin); }),
      Button("Start sync-up", [&] { c.startSync(); }),
      Button("Stop sync-up", [&] { c.stopSync(); }),
      Button("Force sync-up", [&] { c.forceSync(); }),
      Button("Read config", [&] { c.readConfig(); }),
      Button("Save config", [&] { c.saveConfig(); }),
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
  */
/*
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
 
 // const int width = 80;
  const int height = 10;
  auto screen =
      Screen::Create(Dimension::Full(), Dimension::Fixed(height));
  Render(screen, document);
  screen.Print();
  return EXIT_SUCCESS;
  */
}