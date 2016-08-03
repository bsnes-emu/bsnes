#if defined(Hiro_Application)

namespace hiro {

XlibDisplay* pApplication::display = nullptr;

auto pApplication::run() -> void {
  if(Application::state.onMain) {
    while(!Application::state.quit) {
      Application::doMain();
      processEvents();
    }
  } else {
    QApplication::exec();
  }
}

auto pApplication::pendingEvents() -> bool {
  return QApplication::hasPendingEvents();
}

auto pApplication::processEvents() -> void {
  while(pendingEvents()) QApplication::processEvents();
}

auto pApplication::quit() -> void {
  QApplication::quit();
  qtApplication = nullptr;  //note: deleting QApplication will crash libQtGui
}

//this is sadly necessary for things like determining window frame geometry
//obviously, it is used as sparingly as possible
auto pApplication::syncX() -> void {
  for(auto n : range(8)) {
    QApplication::syncX();
    Application::processEvents();
    usleep(2000);
  }
}

auto pApplication::initialize() -> void {
  display = XOpenDisplay(0);

  static int argc = 1;
  static char* argv[] = {new char[8], nullptr};
  strcpy(argv[0], "hiro");
  char** argvp = argv;

  qtApplication = new QApplication(argc, argvp);

  pKeyboard::initialize();
}

}

#endif
