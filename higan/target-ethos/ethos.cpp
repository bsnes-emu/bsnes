#include "ethos.hpp"
#include "bootstrap.cpp"
#include "resource/resource.cpp"

Program *program = nullptr;
DSP dspaudio;

Emulator::Interface& system() {
  if(program->active == nullptr) throw;
  return *program->active;
}

bool Program::focused() {
  return config->input.focusAllow || presentation->focused();
}

//look for file in executable path; if not found, use user path instead
//this allows configuration files to be placed in either location
string Program::path(const string &filename) {
  string path = {basepath, filename};
  if(file::exists(path)) return path;
  if(directory::exists(path)) return path;
  return {userpath, filename};
}

void Program::main() {
  inputManager->poll();
  utility->updateStatus();
  autopause = config->input.focusPause && presentation->focused() == false;

  if(active == nullptr || system().loaded() == false || pause || autopause) {
    audio.clear();
    usleep(20 * 1000);
    return;
  }

  system().run();
}

Program::Program(int argc, char **argv) {
  ananke.open("ananke");

  program = this;
  pause = false;
  autopause = false;

  basepath = dir(realpath(argv[0]));
  userpath = {nall::configpath(), "higan/"};
  directory::create(userpath);

  bootstrap();
  active = nullptr;

  if(Intrinsics::platform() == Intrinsics::Platform::OSX) {
    normalFont = Font::sans(12);
    boldFont = Font::sans(12, "Bold");
    titleFont = Font::sans(24, "Bold");
    monospaceFont = Font::monospace(8);
  } else {
    normalFont = Font::sans(8);
    boldFont = Font::sans(8, "Bold");
    titleFont = Font::sans(16, "Bold");
    monospaceFont = Font::monospace(8);
  }

  config = new Configuration;
  video.driver(config->video.driver);
  audio.driver(config->audio.driver);
  input.driver(config->input.driver);

  utility = new Utility;
  inputManager = new InputManager;
  windowManager = new WindowManager;
  browser = new Browser;
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
  windowManager->loadGeometry();
  presentation->setVisible();

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
  browser->saveConfiguration();
  inputManager->saveConfiguration();
  windowManager->saveGeometry();

  ananke.close();
}

int main(int argc, char **argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  Application::setName("higan");
  Application::Cocoa::onQuit = &Application::quit;
  new Program(argc, argv);
  delete program;
  return 0;
}
