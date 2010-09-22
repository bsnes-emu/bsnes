#include "base.hpp"
#include "interface.cpp"
#include "config.cpp"
Application application;

void Application::main(int argc, char **argv) {
  initialize_arguments(argc, argv);
  config.create();

  char temp[PATH_MAX];
  config.path.base = realpath(argv[0], temp);
  config.path.base.transform("\\", "/");
  config.path.base = dir(config.path.base);
  config.path.user = userpath(temp);
  config.path.user.transform("\\", "/");
  if(strend(config.path.user, "/") == false) config.path.user.append("/");
  config.path.user.append(".bsnes/");
  config.load();
  config.save();

  #if defined(PHOENIX_WINDOWS)
  proportionalFont.create("Tahoma", 8);
  proportionalFontBold.create("Tahoma", 8, Font::Style::Bold);
  monospaceFont.create("Courier New", 8);
  #else
  proportionalFont.create("Sans", 8);
  proportionalFontBold.create("Tahoma", 8, Font::Style::Bold);
  monospaceFont.create("Liberation Mono", 8);
  #endif

  palette.update();
  mainWindow.create();
  videoSettingsWindow.create();
  advancedSettingsWindow.create();
  mainWindow.setVisible();
  while(os.pending()) os.run();

  if(config.video.driver == "") config.video.driver = video.default_driver();
  video.driver(config.video.driver);
  video.set(Video::Handle, mainWindow.viewport.handle());
  video.set(Video::Synchronize, config.video.synchronize);
  video.set(Video::Filter, (unsigned)Video::FilterLinear);
  if(video.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize video.");
    video.driver("None");
    video.init();
  }

  if(config.audio.driver == "") config.audio.driver = audio.default_driver();
  audio.driver(config.audio.driver);
  audio.set(Audio::Handle, mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, config.audio.synchronize);
  audio.set(Audio::Frequency, (unsigned)32000);
  if(audio.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize audio.");
    audio.driver("None");
    audio.init();
  }

  if(config.input.driver == "") config.input.driver = video.default_driver();
  input.driver(config.input.driver);
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
  foreach(window, windows) window->setVisible(false);
  while(os.pending()) os.run();
  SNES::system.term();
  config.save();

  video.term();
  audio.term();
  input.term();
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}
