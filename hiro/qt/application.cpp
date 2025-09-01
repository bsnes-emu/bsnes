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
  //qt6 has no means to spin the event loop indefinitely
  //this is likely to prevent bugs where events produced during the loop spin forever
  //lets match hiro gtk and spin for a max of 50ms
  //note that this overload of processEvents *will* spin, but it does exit if the queue is empty
  QApplication::processEvents(QEventLoop::AllEvents, 50);
}

auto pApplication::quit() -> void {
  QApplication::quit();
  qtApplication = nullptr;  //note: deleting QApplication will crash libQtGui

  #if defined(DISPLAY_XORG)
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
  #endif
}

auto pApplication::setScreenSaver(bool screenSaver) -> void {
  #if defined(DISPLAY_XORG)
  if(state().screenSaverXDG && state().screenSaverWindow) {
    //when invoking this command on Linux under Xfce, the follow message is written to the terminal:
    //"org.freedesktop.DBus.Error.ServiceUnknown: The name org.gnome.SessionManager was not provided by any .service files"
    //to silence this message, stdout and stderr are redirected to /dev/null while invoking this command.
    auto fd = open("/dev/null", O_NONBLOCK);
    auto fo = dup(STDOUT_FILENO);
    auto fe = dup(STDERR_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    invoke("xdg-screensaver", screenSaver ? "resume" : "suspend", string{"0x", hex(state().screenSaverWindow)});
    dup2(fo, STDOUT_FILENO);
    dup2(fe, STDERR_FILENO);
    close(fd);
    close(fo);
    close(fe);
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
  XInitThreads();
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
    XStoreName(state().display, state().screenSaverWindow, "hiro-screen-saver-window");
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

  // Creating the QApplication causes Qt to set the locale from the environment.
  // Set the locale for LC_NUMERIC back to "C". It is expected to be "C" for
  // the purpose of various string formatting and parsing operations.
  setlocale(LC_NUMERIC, "C");

  pKeyboard::initialize();
}

}

#endif
