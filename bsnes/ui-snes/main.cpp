#include "base.hpp"
#include "interface.cpp"
#include "config.cpp"
nall::DSP dspaudio;
Application application;

void Application::main(int argc, char **argv) {
  #if defined(PLATFORM_WIN)
  utf8_args(argc, argv);
  #endif

  compositorActive = compositor::enabled();

  config.create();
  inputMapper.create();

  path.base = dir(realpath(argv[0]));
  path.user = userpath();
  path.load();
  path.save();

  config.load();
  config.save();

  inputMapper.bind();

  #if defined(PLATFORM_WIN)
  proportionalFont = "Tahoma, 8";
  proportionalFontBold = "Tahoma, 8, Bold";
  monospaceFont = "Lucida Console, 8";
  titleFont = "Segoe Print, 16, Bold";
  #else
  proportionalFont = "Sans, 8";
  proportionalFontBold = "Sans, 8, Bold";
  monospaceFont = "Liberation Mono, 8";
  titleFont = "Sans, 16, Bold Italic";
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
  nssDipWindow.create();
  aboutWindow.create();
  settingsWindow.create();
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
  audio.set(Audio::Latency, config.audio.latency);
  audio.set(Audio::Frequency, config.audio.outputFrequency);
  if(audio.init() == false) {
    MessageWindow::critical(mainWindow, "Failed to initialize audio.");
    audio.driver("None");
    audio.init();
  }

  dspaudio.setPrecision(16);  //16-bit signed audio
  dspaudio.setVolume((double)config.audio.volume / 100.0);
  dspaudio.setBalance((double)((signed)config.audio.balance - 100) / 100.0);
  dspaudio.setFrequency(config.audio.inputFrequency);
  dspaudio.setResampler(DSP::Resampler::Hermite);
  dspaudio.setResamplerFrequency(config.audio.outputFrequency);

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
  if(config.settings.startFullScreen) utility.setFullScreen();

  if(argc == 2) cartridge.loadNormal(argv[1]);

  while(quit == false) {
    OS::processEvents();
    inputMapper.poll();
    utility.updateStatus();

    if(SNES::cartridge.loaded()) {
      if(application.pause == true || (config.settings.focusPolicy == 0 && mainWindow.focused() == false)) {
        audio.clear();
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
  utility.setFullScreen(false);
  saveGeometry();
  foreach(window, windows) window->setVisible(false);
  OS::processEvents();
  SNES::system.term();

  path.save();
  config.save();

  video.term();
  audio.term();
  input.term();

  if(compositorActive) compositor::enable(true);
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
    Geometry configGeometry = {
      (signed)integer(position[0]), (signed)integer(position[1]),
      (unsigned)decimal(position[2]), (unsigned)decimal(position[3])
    };
    Geometry windowGeometry = window->geometry();

    //Windows places minimized windows offscreen at 32000,32000
    //this is a fix for older releases that did not compensate for this
    if(configGeometry.x >= 30000) configGeometry.x = 128;
    if(configGeometry.y >= 30000) configGeometry.y = 128;

    window->setGeometry({
      configGeometry.x, configGeometry.y,
      windowGeometry.width, windowGeometry.height
    //configGeometry.width, configGeometry.height
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
