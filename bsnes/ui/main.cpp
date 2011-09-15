#include "base.hpp"

Application *application = 0;
nall::DSP dspaudio;

void Application::run() {
  interface->input_poll();

  if(interface->loaded() == false) {
    usleep(20 * 1000);
    return;
  }

  interface->run();
}

Application::Application(int argc, char **argv) : quit(false) {
  application = this;
  {
    char path[PATH_MAX];
    auto unused = ::realpath(argv[0], path);
    realpath = path;
    unused = ::userpath(path);
    userpath = path;
  }
  config = new Config;
  interface = new Interface;
  utility = new Utility;

  title = "batch";

  #if defined(PLATFORM_WIN)
  string videoDriver = "Direct3D", audioDriver = "XAudio2", inputDriver = "RawInput";
  normalFont = "Tahoma, 8";
  boldFont = "Tahoma, 8, Bold";
  #else
  string videoDriver = "OpenGL", audioDriver = "PulseAudio", inputDriver = "SDL";
  normalFont = "Sans, 8";
  boldFont = "Sans, 8, Bold";
  #endif

  mainWindow = new MainWindow;
  fileBrowser = new FileBrowser;
  cheatEditor = new CheatEditor;
  utility->setMode(Interface::Mode::None);
  mainWindow->setVisible();

  video.driver(videoDriver);
  video.set(Video::Handle, mainWindow->viewport.handle());
  video.set(Video::Synchronize, true);
  video.set(Video::Filter, 0u);
  video.init();

  audio.driver(audioDriver);
  audio.set(Audio::Handle, mainWindow->viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Latency, 60u);
  audio.set(Audio::Frequency, 48000u);
  audio.init();

  dspaudio.setPrecision(16);
  dspaudio.setVolume(1.0);
  dspaudio.setBalance(0.0);
  dspaudio.setResampler(DSP::Resampler::Average);
  dspaudio.setResamplerFrequency(48000.0);

  input.driver(inputDriver);
  input.set(Input::Handle, mainWindow->viewport.handle());
  input.init();

  if(argc == 2) interface->loadCartridge(argv[1]);

  while(quit == false) {
    OS::processEvents();
    Application::run();
  }

  interface->unloadCartridge();
}

Application::~Application() {
  delete cheatEditor;
  delete fileBrowser;
  delete mainWindow;
  delete utility;
  delete interface;
  delete config;
}

int main(int argc, char **argv) {
  new Application(argc, argv);
  delete application;
  return 0;
}
