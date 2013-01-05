#include "ethos.hpp"
#include "bootstrap.cpp"
#include "resource/resource.cpp"

Application *application = nullptr;
DSP dspaudio;

Emulator::Interface& system() {
  if(application->active == nullptr) throw;
  return *application->active;
}

bool Application::focused() {
  return config->input.focusAllow || presentation->focused();
}

string Application::path(const string &filename) {
  string path = {basepath, filename};
  if(file::exists(path)) return path;
  if(directory::exists(path)) return path;
  return {userpath, filename};
}

void Application::run() {
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

Application::Application(int argc, char **argv) {
  ananke.open("ananke");

  application = this;
  quit = false;
  pause = false;
  autopause = false;

  basepath = dir(realpath(argv[0]));
  userpath = {nall::configpath(), "higan/"};
  directory::create(userpath);

  bootstrap();
  active = nullptr;

  if(Intrinsics::platform() == Intrinsics::Platform::Windows) {
    normalFont = "Tahoma, 8";
    boldFont = "Tahoma, 8, Bold";
    titleFont = "Tahoma, 16, Bold";
    monospaceFont = "Lucida Console, 8";
  } else {
    normalFont = "Sans, 8";
    boldFont = "Sans, 8, Bold";
    titleFont = "Sans, 16, Bold";
    monospaceFont = "Liberation Mono, 8";
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
  driverSettings = new DriverSettings;
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

  if(config->video.startFullScreen) utility->toggleFullScreen();
  OS::processEvents();

  if(argc >= 2) utility->loadMedia(argv[1]);

  while(quit == false) {
    OS::processEvents();
    run();
  }

  utility->unload();
  config->save();
  browser->saveConfiguration();
  inputManager->saveConfiguration();
  windowManager->saveGeometry();

  ananke.close();
}

Application::~Application() {
}

int main(int argc, char **argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  OS::setName("higan");
  new Application(argc, argv);
  delete application;
  return 0;
}
