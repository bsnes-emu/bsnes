#include "base.hpp"
Application application;

#include "interface.cpp"

#include "general/main-window.cpp"

void Application::main(int argc, char **argv) {
  quit = false;

  mainWindow.create();

  mainWindow.setVisible();

  GameBoy::system.init(&interface);

  while(quit == false) {
    OS::run();
  }
}

int main(int argc, char **argv) {
  application.main(argc, argv);

  return 0;
}
