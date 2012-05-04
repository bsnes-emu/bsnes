#include "ethos.hpp"
#include "bootstrap.cpp"

Application *application = nullptr;
DSP dspaudio;

Emulator::Interface& system() {
  struct application_interface_null{};
  if(application->active == nullptr) throw application_interface_null();
  return *application->active;
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
  application = this;
  quit = false;
  pause = false;
  autopause = false;

  char path[PATH_MAX];
  auto unused = ::realpath(argv[0], path);
  basepath = dir(path);
  unused = ::userpath(path);
  userpath = path;
  if(Intrinsics::platform() == Intrinsics::Platform::Windows) {
    userpath.append("ethos/");
  } else {
    userpath.append(".config/ethos/");
  }
  mkdir(userpath, 0755);

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

  video.driver("OpenGL");
  audio.driver("ALSA");
  input.driver("SDL");

  config = new Configuration;
  utility = new Utility;
  inputManager = new InputManager;
  windowManager = new WindowManager;
  browser = new Browser;
  presentation = new Presentation;
  videoSettings = new VideoSettings;
  audioSettings = new AudioSettings;
  inputSettings = new InputSettings;
  hotkeySettings = new HotkeySettings;
  settings = new Settings;
  windowManager->loadGeometry();
  presentation->setVisible();

  video.set(Video::Handle, presentation->viewport.handle());
  if(!video.cap(Video::Depth) || !video.set(Video::Depth, depth = 30u)) {
    video.set(Video::Depth, depth = 24u);
  }
  video.init();

  audio.set(Audio::Handle, presentation->viewport.handle());
  audio.init();

  input.set(Input::Handle, presentation->viewport.handle());
  input.init();

  dspaudio.setPrecision(16);
  dspaudio.setBalance(0.0);
  dspaudio.setFrequency(96000);

  utility->synchronizeRuby();

  while(quit == false) {
    OS::processEvents();
    run();
  }

  if(active && system().loaded()) utility->unload();
  config->save();
  browser->saveConfiguration();
  inputManager->saveConfiguration();
  windowManager->saveGeometry();
}

Application::~Application() {
}

int main(int argc, char **argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif
  new Application(argc, argv);
  delete application;
  return 0;
}
