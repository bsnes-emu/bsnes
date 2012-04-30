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

  if(active == nullptr || system().loaded() == false) {
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

  utility = new Utility;
  inputManager = new InputManager;
  browser = new Browser;
  presentation = new Presentation;
  videoSettings = new VideoSettings;
  audioSettings = new AudioSettings;
  inputSettings = new InputSettings;
  hotkeySettings = new HotkeySettings;
  settings = new Settings;

  presentation->setVisible();

  video.driver("OpenGL");
  video.set(Video::Handle, presentation->viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Depth, 24u);
  video.init();

  audio.driver("ALSA");
  audio.set(Audio::Handle, presentation->viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Latency, 80u);
  audio.set(Audio::Frequency, 48000u);
  audio.init();

  input.driver("SDL");
  input.set(Input::Handle, presentation->viewport.handle());
  input.init();

  dspaudio.setPrecision(16);
  dspaudio.setVolume(2.0);
  dspaudio.setBalance(0.0);
  dspaudio.setResampler(DSP::ResampleEngine::Linear);
  dspaudio.setResamplerFrequency(48000u);

  while(quit == false) {
    OS::processEvents();
    run();
  }

  browser->saveConfiguration();
  inputManager->saveConfiguration();
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
