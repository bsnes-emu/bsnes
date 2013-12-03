#include "ethos.hpp"
#include "bootstrap.cpp"
#include "resource/resource.cpp"

Program* program = nullptr;
DSP dspaudio;

Emulator::Interface& system() {
  if(program->active == nullptr) throw;
  return *program->active;
}

bool Program::focused() {
  return config->input.focus.allow || presentation->focused();
}

string Program::path(string name) {
  string path = {basepath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {userpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {sharedpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  return {userpath, name};
}

void Program::main() {
  inputManager->poll();
  utility->updateStatus();
  autopause = config->input.focus.pause && presentation->focused() == false;

  if(active == nullptr || system().loaded() == false || pause || autopause) {
    audio.clear();
    usleep(20 * 1000);
    return;
  }

  system().run();
}

Program::Program(int argc, char** argv) {
  ananke.open("ananke");

  program = this;
  pause = false;
  autopause = false;

  basepath = dir(realpath(argv[0]));
  userpath = {nall::configpath(), "higan/"};
  sharedpath = {nall::sharedpath(), "higan/"};
  directory::create(userpath);

  bootstrap();
  active = nullptr;

  normalFont = Font::sans(8);
  boldFont = Font::sans(8, "Bold");
  titleFont = Font::sans(16, "Bold");
  monospaceFont = Font::monospace(8);

  config = new ConfigurationSettings;
  video.driver(config->video.driver);
  audio.driver(config->audio.driver);
  input.driver(config->input.driver);

  utility = new Utility;
  inputManager = new InputManager;
  windowManager = new WindowManager;
  libraryManager = new LibraryManager;
  presentation = new Presentation;
  dipSwitches = new DipSwitches;
  videoSettings = new VideoSettings;
  audioSettings = new AudioSettings;
  inputSettings = new InputSettings;
  hotkeySettings = new HotkeySettings;
  timingSettings = new TimingSettings;
  serverSettings = new ServerSettings;
  advancedSettings = new AdvancedSettings;
  settings = new Settings;
  cheatDatabase = new CheatDatabase;
  cheatEditor = new CheatEditor;
  stateManager = new StateManager;
  tools = new Tools;
  windowManager->loadGeometry();
  presentation->setVisible();
  utility->resize();

  if(argc == 1 && config->library.showOnStartup) libraryManager->show();

  video.set(Video::Handle, presentation->viewport.handle());
  if(!video.cap(Video::Depth) || !video.set(Video::Depth, depth = 30u)) {
    video.set(Video::Depth, depth = 24u);
  }
  if(video.init() == false) { video.driver("None"); video.init(); }

  audio.set(Audio::Handle, presentation->viewport.handle());
  if(audio.init() == false) { audio.driver("None"); audio.init(); }

  input.set(Input::Handle, presentation->viewport.handle());
  if(input.init() == false) { input.driver("None"); input.init(); }

  dspaudio.setPrecision(16);
  dspaudio.setBalance(0.0);
  dspaudio.setFrequency(96000);

  utility->synchronizeRuby();
  utility->updateShader();

  if(config->video.startFullScreen && argc >= 2) utility->toggleFullScreen();
  Application::processEvents();

  if(argc >= 2) utility->loadMedia(argv[1]);

  Application::main = {&Program::main, this};
  Application::run();

  utility->unload();
  config->save();
  inputManager->saveConfiguration();
  windowManager->saveGeometry();

  ananke.close();
}

int main(int argc, char** argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  Application::setName("higan");

  Application::Windows::onModalBegin = [&] {
    audio.clear();
  };

  Application::Cocoa::onActivate = [&] {
    presentation->setVisible();
  };

  Application::Cocoa::onAbout = [&] {
    MessageWindow()
    .setTitle({"About ", Emulator::Name})
    .setText({
      Emulator::Name, " v", Emulator::Version, "\n",
      Emulator::Profile, " Profile\n",
      "Author: ", Emulator::Author, "\n",
      "License: ", Emulator::License, "\n",
      "Website: ", Emulator::Website
    })
    .information();
  };

  Application::Cocoa::onPreferences = [&] {
    settings->setVisible();
  };

  Application::Cocoa::onQuit = [&] {
    Application::quit();
  };

  new Program(argc, argv);
  delete program;
  return 0;
}
