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

  config.path.base = dir(realpath(argv[0]));
  config.path.user = userpath();

  config.load();
  config.save();
  if(config.path.current == "") config.path.current = config.path.base;
  inputMapper.bind();

  #if defined(PLATFORM_WIN)
  proportionalFont.setFamily("Tahoma");
  proportionalFont.setSize(8);

  proportionalFontBold.setFamily("Tahoma");
  proportionalFontBold.setSize(8);
  proportionalFontBold.setBold();

  monospaceFont.setFamily("Lucida Console");
  monospaceFont.setSize(8);
  #else
  proportionalFont.setFamily("Sans");
  proportionalFont.setSize(8);

  proportionalFontBold.setFamily("Sans");
  proportionalFontBold.setSize(8);
  proportionalFontBold.setBold();

  monospaceFont.setFamily("Liberation Mono");
  monospaceFont.setSize(8);
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
  cheatDatabase.create();
  stateManager.create();
  #if defined(DEBUGGER)
  debugger.create();
  #endif

  loadGeometry();
  saveGeometry();

  utility.setScale(config.video.scale);
  mainWindow.setVisible();
  OS::processEvents();

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
  utility.setFilter();
  utility.setShader();

  if(argc == 2) cartridge.loadNormal(argv[1]);

  while(quit == false) {
    OS::processEvents();
    inputMapper.poll();
    utility.updateStatus();

    if(SNES::cartridge.loaded()) {
      if(application.pause == true || (config.settings.focusPolicy == 0 && mainWindow.focused() == false)) {
        usleep(20 * 1000);
        continue;
      }
      #if defined(DEBUGGER)
      debugger.run();
      #else
      SNES::system.run();
      #endif
    } else {
      usleep(20 * 1000);
    }
  }

  cartridge.unload();
  saveGeometry();
  foreach(window, windows) window->setVisible(false);
  OS::processEvents();
  SNES::system.term();
  config.save();

  video.term();
  audio.term();
  input.term();
}

void Application::addWindow(TopLevelWindow *window, const string &name, const string &position) {
  windows.append(window);
  window->name = name;
  window->position = position;
  window->setWidgetFont(proportionalFont);
  geometryConfig.attach(window->position, window->name);
}

Application::Application() {
  pause = false;
  quit = false;
}

int main(int argc, char **argv) {
  application.main(argc, argv);
  return 0;
}

void Application::loadGeometry() {
  geometryConfig.load(path.home("geometry.cfg"));
  foreach(window, windows) {
    lstring position;
    position.split(",", window->position);
    Geometry geom = window->geometry();
    window->setGeometry({
      (signed)integer(position[0]), (signed)integer(position[1]),
      geom.width, geom.height
    //(unsigned)decimal(position[2]), (unsigned)decimal(position[3])
    });
  }
}

void Application::saveGeometry() {
  foreach(window, windows) {
    Geometry geom = window->geometry();
    window->position = { geom.x, ",", geom.y, ",", geom.width, ",", geom.height };
  }
  geometryConfig.save(path.home("geometry.cfg"));
}
