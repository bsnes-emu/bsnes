#include "higan.hpp"

struct Presentation : Window {
  MenuBar menuBar{this};
  Menu menuSystem{&menuBar};
  StatusBar statusBar{this};

  Presentation() {
    menuSystem.setText("System");
    onClose(&Application::quit);

    setBackgroundColor({0, 0, 0});
    setTitle({Emulator::Name, " v", Emulator::Version});
    setSize({640, 480});
    setCentered();
    setVisible();
  }
};

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("higan");
  new Presentation;
  Application::run();
}
