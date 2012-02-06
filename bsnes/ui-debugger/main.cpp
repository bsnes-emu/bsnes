#include "base.hpp"

Application *application = nullptr;

Application::Application(int argc, char **argv) {
  application = this;
  quit = false;

  {
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
  }

  if(Intrinsics::platform() == Intrinsics::Platform::Windows) {
    proportionalFont = "Tahoma, 8";
    proportionalFontBold = "Tahoma, 8, Bold";
    monospaceFont = "Lucida Console, 8";
  } else {
    proportionalFont = "Sans, 8";
    proportionalFontBold = "Sans, 8, Bold";
    monospaceFont = "Liberation Mono, 8";
  }

  string filename;
  if(argc >= 2) filename = argv[1];
  if(!file::exists(filename)) filename = "/media/sdb1/root/cartridges/SNES - Archived/zelda_us.sfc";
  if(!file::exists(filename)) filename = DialogWindow::fileOpen(Window::None, "", "SNES images (*.sfc)");
  if(!file::exists(filename)) return;

  interface = new Interface;
  consoleWindow = new ConsoleWindow;
  videoWindow = new VideoWindow;

  videoWindow->setVisible();
  consoleWindow->setVisible();

  if(audio.init() == false) {
    audio.driver("None");
    audio.init();
  }
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Frequency, 32000u);

  if(interface->loadCartridge(filename) == false) return;

  while(quit == false) {
    OS::processEvents();
    SNES::system.run();
  }

  interface->saveMemory();
}

Application::~Application() {
  delete videoWindow;
  delete consoleWindow;
  delete interface;
}

int main(int argc, char **argv) {
  new Application(argc, argv);
  delete application;
  return 0;
}
