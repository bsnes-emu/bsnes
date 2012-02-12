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

  string foldername;
  if(argc >= 2) foldername = argv[1];
  if(!directory::exists(foldername)) foldername = "/media/sdb1/root/cartridges/The Legend of Zelda - A Link to the Past (US).sfc/";
  if(!directory::exists(foldername)) foldername = DialogWindow::folderSelect(Window::None, "");
  if(!foldername.endswith(".sfc/")) return;
  lstring contents = directory::files(foldername, "*.sfc");
  if(contents.size() != 1) return;
  string filename = { foldername, contents[0] };
  if(!file::exists(filename)) return;

  interface = new Interface;
  debugger = new Debugger;
  tracer = new Tracer;
  consoleWindow = new ConsoleWindow;
  aboutWindow = new AboutWindow;
  videoWindow = new VideoWindow;
  cpuDebugger = new CPUDebugger;
  smpDebugger = new SMPDebugger;
  memoryEditor = new MemoryEditor;
  breakpointEditor = new BreakpointEditor;

  consoleWindow->setVisible();
  videoWindow->setVisible();
  consoleWindow->setFocused();

  if(audio.init() == false) {
    audio.driver("None");
    audio.init();
  }
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Frequency, 32000u);

  if(interface->loadCartridge(filename) == false) return;
  cpuDebugger->updateDisassembly();
  smpDebugger->updateDisassembly();
  memoryEditor->selectSource();

  while(quit == false) {
    OS::processEvents();
    debugger->run();
  }

  interface->saveMemory();
}

Application::~Application() {
  exit(0);
  delete breakpointEditor;
  delete memoryEditor;
  delete smpDebugger;
  delete cpuDebugger;
  delete videoWindow;
  delete aboutWindow;
  delete consoleWindow;
  delete tracer;
  delete debugger;
  delete interface;
}

int main(int argc, char **argv) {
  new Application(argc, argv);
  delete application;
  return 0;
}
