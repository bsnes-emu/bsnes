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
      userpath.append("laevateinn/");
    } else {
      userpath.append(".config/laevateinn/");
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

  settings = new Settings;
  settings->load();

  string foldername;
  if(argc >= 2) foldername = argv[1];
//if(!directory::exists(foldername)) foldername = "/media/sdb1/root/cartridges/The Legend of Zelda - A Link to the Past (US).sfc/";
  if(!directory::exists(foldername)) foldername = DialogWindow::folderSelect(Window::None, settings->folderpath);
  if(!foldername.endswith(".sfc/")) return;
  if(!directory::exists(foldername)) return;

  //save path for later; remove cartridge name from path
  settings->folderpath = foldername;
  settings->folderpath.rtrim<1>("/");
  settings->folderpath = dir(settings->folderpath);

  interface = new Interface;
  debugger = new Debugger;
  tracer = new Tracer;
  windowManager = new WindowManager;
  consoleWindow = new ConsoleWindow;
  aboutWindow = new AboutWindow;
  videoWindow = new VideoWindow;
  cpuDebugger = new CPUDebugger;
  cpuRegisterEditor = new CPURegisterEditor;
  smpDebugger = new SMPDebugger;
  smpRegisterEditor = new SMPRegisterEditor;
  memoryEditor = new MemoryEditor;
  breakpointEditor = new BreakpointEditor;
  propertiesViewer = new PropertiesViewer;
  vramViewer = new VRAMViewer;

  windowManager->loadGeometry();
  consoleWindow->setVisible();
  videoWindow->setVisible();
  consoleWindow->setFocused();

  if(audio.init() == false) {
    audio.driver("None");
    audio.init();
  }
  audio.set(Audio::Synchronize, settings->synchronizeAudio);
  audio.set(Audio::Frequency, 32000u);

  if(interface->loadCartridge(foldername) == false) return;
  cpuDebugger->updateDisassembly();
  smpDebugger->updateDisassembly();
  memoryEditor->selectSource();
  propertiesViewer->updateProperties();
  vramViewer->updateTiles();

  while(quit == false) {
    OS::processEvents();
    debugger->run();
  }

  interface->saveMemory();
  windowManager->saveGeometry();
  settings->save();
}

Application::~Application() {
  delete vramViewer;
  delete propertiesViewer;
  delete breakpointEditor;
  delete memoryEditor;
  delete smpRegisterEditor;
  delete smpDebugger;
  delete cpuRegisterEditor;
  delete cpuDebugger;
  delete videoWindow;
  delete aboutWindow;
  delete consoleWindow;
  delete windowManager;
  delete tracer;
  delete debugger;
  delete interface;
  delete settings;
}

int main(int argc, char **argv) {
  new Application(argc, argv);
  delete application;
  return 0;
}
