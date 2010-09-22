#include "base.hpp"
#include "interface.cpp"
Application application;

#if defined(PLATFORM_WIN)
static string VideoDriver = "Direct3D";
static string AudioDriver = "DirectSound";
static string InputDriver = "RawInput";
#elif defined(PLATFORM_X)
static string VideoDriver = "OpenGL";
static string AudioDriver = "ALSA";
static string InputDriver = "SDL";
#endif

void Application::main(int argc, char **argv) {
  #if defined(PHOENIX_WINDOWS)
  font.create("Tahoma", 8);
  #else
  font.create("Sans", 8);
  #endif

  mainWindow.create();
  videoSettingsWindow.create();
  mainWindow.setVisible();
  while(os.pending()) os.run();

  video.driver(VideoDriver);
  video.set(Video::Handle, mainWindow.viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Filter, (unsigned)Video::FilterLinear);
  if(video.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize video.");
    video.driver("None");
    video.init();
  }

  audio.driver(AudioDriver);
  audio.set(Audio::Handle, mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, false);
  audio.set(Audio::Frequency, (unsigned)32000);
  if(audio.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize audio.");
    audio.driver("None");
    audio.init();
  }

  input.driver(InputDriver);
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
    while(os.pending()) os.run();

    if(SNES::cartridge.loaded()) {
      SNES::system.run();
    } else {
      usleep(20 * 1000);
    }
  }

  cartridge.unload();
  mainWindow.setVisible(false);
  while(os.pending()) os.run();
  SNES::system.term();

  video.term();
  audio.term();
  input.term();
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}
