#if defined(Hiro_Application)

namespace hiro {

auto pApplication::modal() -> bool {
  return Application::state().modal > 0;
}

auto pApplication::run() -> void {
  if(Application::state().onMain) {
    while(!Application::state().quit) {
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

  if(state().display) {
    if(state().screenSaverXDG && state().screenSaverWindow) {
      //this needs to run synchronously, so that XUnmapWindow() won't happen before xdg-screensaver is finished
      execute("xdg-screensaver", "resume", string{"0x", hex(state().screenSaverWindow)});
      XUnmapWindow(state().display, state().screenSaverWindow);
      state().screenSaverWindow = 0;
    }
    XCloseDisplay(state().display);
    state().display = nullptr;
  }
}

auto pApplication::setScreenSaver(bool screenSaver) -> void {
  #if defined(DISPLAY_XORG)
  if(state().screenSaverXDG && state().screenSaverWindow) {
    invoke("xdg-screensaver", screenSaver ? "resume" : "suspend", string{"0x", hex(state().screenSaverWindow)});
  }
  #endif
}

auto pApplication::state() -> State& {
  static State state;
  return state;
}

//this is sadly necessary for things like determining window frame geometry
//obviously, it is used as sparingly as possible
auto pApplication::synchronize() -> void {
  for(auto n : range(8)) {
    #if HIRO_QT==4 && defined(DISPLAY_XORG)
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

  #if defined(DISPLAY_XORG)
  state().display = XOpenDisplay(nullptr);
  state().screenSaverXDG = (bool)execute("xdg-screensaver", "--version").output.find("xdg-screensaver");

  if(state().screenSaverXDG) {
    auto screen = DefaultScreen(state().display);
    auto rootWindow = RootWindow(state().display, screen);
    XSetWindowAttributes attributes{};
    attributes.background_pixel = BlackPixel(state().display, screen);
    attributes.border_pixel = 0;
    attributes.override_redirect = true;
    state().screenSaverWindow = XCreateWindow(state().display, rootWindow,
      0, 0, 1, 1, 0, DefaultDepth(state().display, screen),
      InputOutput, DefaultVisual(state().display, screen),
      CWBackPixel | CWBorderPixel | CWOverrideRedirect, &attributes
    );
  //note: hopefully xdg-screensaver does not require the window to be mapped ...
  //if it does, we're in trouble: a small 1x1 black pixel window will be visible in said case
    XMapWindow(state().display, state().screenSaverWindow);
    XFlush(state().display);
  }
  #endif

  static auto name = Application::state().name ? Application::state().name : string{"hiro"};

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
