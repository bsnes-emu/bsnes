#include "base.hpp"
#include "interface.cpp"
#include "config.cpp"
Application application;

void Application::main(int argc, char **argv) {
  #if defined(PLATFORM_WIN)
  utf8_args(argc, argv);
  #endif
  config.create();
  inputMapper.create();

  config.path.base = realpath(argv[0]);
  config.path.user = { userpath(), ".bsnes/" };

  config.load();
  config.save();
  if(config.path.current == "") config.path.current = config.path.base;
  inputMapper.bind();

  #if defined(PHOENIX_WINDOWS)
  proportionalFont.create("Tahoma", 8);
  proportionalFontBold.create("Tahoma", 8, Font::Style::Bold);
  monospaceFont.create("Courier New", 8);
  #else
  proportionalFont.create("Sans", 8);
  proportionalFontBold.create("Tahoma", 8, Font::Style::Bold);
  monospaceFont.create("Liberation Mono", 8);
  #endif

  SNES::system.init(&interface);

  if(config.video.driver == "") config.video.driver = video.default_driver();
  if(config.audio.driver == "") config.audio.driver = audio.default_driver();
  if(config.input.driver == "") config.input.driver = input.default_driver();

  palette.update();

  mainWindow.create();
  fileBrowser.create();
  singleSlotLoader.create();
  doubleSlotLoader.create();
  videoSettings.create();
  audioSettings.create();
  inputSettings.create();
  advancedSettings.create();
  cheatEditor.create();
  stateManager.create();
  loadGeometry();
  saveGeometry();

  utility.setScale(config.video.scale);
  mainWindow.setVisible();
  OS::run();

  video.driver(config.video.driver);
  video.set(Video::Handle, mainWindow.viewport.handle());
  video.set(Video::Synchronize, config.video.synchronize);
  video.set(Video::Filter, (unsigned)config.video.smooth);
  if(video.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize video.");
    video.driver("None");
    video.init();
  }

  audio.driver(config.audio.driver);
  audio.set(Audio::Handle, mainWindow.viewport.handle());
  audio.set(Audio::Synchronize, config.audio.synchronize);
  audio.set(Audio::Volume, config.audio.volume);
  audio.set(Audio::Latency, config.audio.latency);
  audio.set(Audio::Frequency, config.audio.outputFrequency);
  audio.set(Audio::Resample, true);
  audio.set(Audio::ResampleRatio, (double)config.audio.inputFrequency / (double)config.audio.outputFrequency);
  if(audio.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize audio.");
    audio.driver("None");
    audio.init();
  }

  input.driver(config.input.driver);
  input.set(Input::Handle, mainWindow.viewport.handle());
  if(input.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize input.");
    input.driver("None");
    input.init();
  }

  utility.setControllers();
  utility.setShader();

  if(argc == 2) cartridge.loadNormal(argv[1]);

  while(quit == false) {
    OS::run();
    inputMapper.poll();
    utility.updateStatus();

    if(SNES::cartridge.loaded()) {
      //pause emulator when main window is inactive?
      if(config.settings.focusPolicy == 0) {
        if(mainWindow.focused() == false) {
          usleep(20 * 1000);
          continue;
        }
      }
      SNES::system.run();
    } else {
      usleep(20 * 1000);
    }
  }

  cartridge.unload();
  saveGeometry();
  foreach(window, windows) window->setVisible(false);
  OS::run();
  SNES::system.term();
  config.save();

  video.term();
  audio.term();
  input.term();
}

void Application::addWindow(TopLevelWindow *window, const string &name, const string &position) {
  windows.append(window);
  window->setDefaultFont(proportionalFont);
  window->name = name;
  window->position = position;
  geometryConfig.attach(window->position, window->name);
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}

void Application::loadGeometry() {
  geometryConfig.load(string(config.path.user, "bsnes-phoenix-geometry.cfg"));
  foreach(window, windows) {
    lstring position;
    position.split(",", window->position);
    Geometry geom = window->geometry();
    window->setGeometry(strunsigned(position[0]), strunsigned(position[1]), geom.width, geom.height);
  }
}

void Application::saveGeometry() {
  foreach(window, windows) {
    Geometry geom = window->geometry();
    window->position = { geom.x, ",", geom.y };
  }
  geometryConfig.save(string(config.path.user, "bsnes-phoenix-geometry.cfg"));
}
