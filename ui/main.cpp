#include "base.hpp"
Application application;

#include "interface.cpp"

#include "general/main-window.cpp"

void Application::main(int argc, char **argv) {
  quit = false;

  mainWindow.create();

  mainWindow.setVisible();

  GameBoy::system.init(&interface);

  unsigned frameCounter = 0;
  time_t timeCounter = time(0);

  while(quit == false) {
    OS::run();

    if(GameBoy::cartridge.loaded()) {
      GameBoy::system.run();

      frameCounter++;
      time_t currentTime = time(0);
      if(currentTime != timeCounter) {
        timeCounter = currentTime;
        mainWindow.setStatusText({ "FPS: ", frameCounter });
        frameCounter = 0;
      }
    }
  }
}

int main(int argc, char **argv) {
  application.main(argc, argv);

  return 0;
}
