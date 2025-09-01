#if defined(Hiro_Application)

namespace hiro {

auto pApplication::exit() -> void {
  quit();
  ::exit(EXIT_SUCCESS);
}

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

auto pApplication::processEvents() -> void {
  //qt6 has no means to spin the event loop indefinitely anymore
  //this is likely to prevent bugs where events produced during the loop spin forever
  //lets match hiro gtk and spin for a max of 50ms
  //note that this overload of processEvents *will* spin, but it does exit if the queue is empty
  //so it basically gives us the behavior from before, but with a 50ms cap, which is probably good
  QApplication::processEvents(QEventLoop::AllEvents, 50);
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
    QApplication::sync();
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
    XStoreName(state().display, state().screenSaverWindow, "hiro screensaver-prevention window");
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
