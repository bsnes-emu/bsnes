#include "ethos.hpp"

Application *application = nullptr;

string Application::path(const string &filename) {
  string path = {basepath, filename};
  if(file::exists(path)) return path;
  return {userpath, filename};
}

void Application::run() {
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
    userpath.append("bsnes/");
  } else {
    userpath.append(".config/bsnes/");
  }
  mkdir(userpath, 0755);

  interface = new Interface;

  auto gba = new GameBoyAdvance::Interface;
  gba->callback.videoColor = {&Interface::videoColor, interface};
  gba->callback.videoRefresh = {&Interface::videoRefresh, interface};
  gba->callback.audioSample = {&Interface::audioSample, interface};
  gba->callback.inputPoll = {&Interface::inputPoll, interface};
  gba->updatePalette();
  emulators.append(gba);

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

  filestream bios{"/home/byuu/.config/bsnes/Game Boy Advance.sys/bios.rom"};
  gba->load(1, bios);

  string manifest;
  manifest.readfile("/media/sdb1/root/cartridges/Game Boy Advance/Super Mario Advance (US).gba/manifest.xml");
  filestream fs{"/media/sdb1/root/cartridges/Game Boy Advance/Super Mario Advance (US).gba/program.rom"};
  gba->load(0, fs, manifest);
  gba->power();

  videoWindow = new VideoWindow;

  videoWindow->setVisible();

  video.driver("OpenGL");
  video.set(Video::Handle, videoWindow->viewport.handle());
  video.set(Video::Synchronize, false);
  video.set(Video::Depth, 24u);
  video.init();

  audio.driver("ALSA");
  audio.set(Audio::Handle, videoWindow->viewport.handle());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Latency, 80u);
  audio.set(Audio::Frequency, 32768u);
  audio.init();

  input.driver("SDL");
  input.set(Input::Handle, videoWindow->viewport.handle());
  input.init();

  while(quit == false) {
    OS::processEvents();
    gba->run();
  }
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
