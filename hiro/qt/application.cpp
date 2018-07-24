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

auto pApplication::setScreenSaver(bool screenSaver) -> void {
  //TODO: not implemented
}

//this is sadly necessary for things like determining window frame geometry
//obviously, it is used as sparingly as possible
auto pApplication::syncX() -> void {
  for(auto n : range(8)) {
    #if HIRO_QT==4
    QApplication::syncX();
    #elif HIRO_QT==5
    QApplication::sync();
    #endif
    Application::processEvents();
    usleep(2000);
  }
}

auto pApplication::initialize() -> void {
  #if HIRO_QT==5 && defined(PLATFORM_BSD)
  setenv("QTCOMPOSE", "/usr/local/lib/X11/locale/", 0);
  #endif

  display = XOpenDisplay(0);

  auto name = Application::state.name ? Application::state.name : string{"hiro"};

  //QApplication stores references to argc;
  //and will access them after pApplication::initialize() returns
  static int argc = 1;
  static char* argv[] = {name.get(), nullptr};
  static char** argvp = argv;
  qtApplication = new QApplication(argc, argvp);

  pKeyboard::initialize();
}

}

#endif
