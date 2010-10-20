#include "base.hpp"
#include "interface.cpp"
Application application;

void Application::main(int argc, char **argv) {
  mainWindow.create();
  while(os.pending()) os.run();

  video.driver("OpenGL");
  video.set(Video::Handle, mainWindow.viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Filter, (unsigned)Video::FilterLinear);
  if(video.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize video.");
    video.driver("None");
    video.init();
  }

  audio.driver("ALSA");
  audio.set(Audio::Handle, mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, false);
  audio.set(Audio::Frequency, (unsigned)32000);
  if(audio.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize audio.");
    audio.driver("None");
    audio.init();
  }

  input.driver("SDL");
  input.set(Input::Handle, mainWindow.viewport.handle());
  if(input.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize input.");
    input.driver("None");
    input.init();
  }

  SNES::system.init(&interface);
  if(argc == 2) {
    cartridge.loadNormal(argv[1]);
    SNES::system.power();
  }

  while(quit == false) {
    if(os.pending()) os.run();

    if(SNES::cartridge.loaded()) {
      SNES::system.run();
    } else {
      usleep(20 * 1000);
    }
  }

  mainWindow.setVisible(false);
  while(os.pending()) os.run();
  SNES::system.term();

  exit(0);
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}
