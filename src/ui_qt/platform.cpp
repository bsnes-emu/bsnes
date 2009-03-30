//platform-specific wrappers

#if defined(_WIN32)
  //Windows 32-bit and 64-bit
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <nall/utf8.hpp>

  char* realpath(const char *filename, char *resolvedname) {
    wchar_t fn[_MAX_PATH] = L"";
    _wfullpath(fn, nall::utf16_t(filename), _MAX_PATH);
    strcpy(resolvedname, nall::utf8_t(fn));
    return resolvedname;
  }

  char* userpath(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, fp);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }

  char* getcwd(char *path) {
    wchar_t fp[_MAX_PATH] = L"";
    _wgetcwd(fp, _MAX_PATH);
    strcpy(path, nall::utf8_t(fp));
    return path;
  }

  int mkdir(const char *path) {
    return _wmkdir(nall::utf16_t(path));
  }

  void initargs(int &argc, char **&argv) {
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    argv = new char*[argc];
    for(unsigned i = 0; i < argc; i++) {
      argv[i] = new char[_MAX_PATH];
      strcpy(argv[i], nall::utf8_t(wargv[i]));
    }
  }

  bool Application::App::winEventFilter(MSG *msg, long *result) {
    if(msg->message == WM_SYSCOMMAND) {
      if(msg->wParam == SC_SCREENSAVE || msg->wParam == SC_MONITORPOWER) {
        *result = 0;
        return true;
      }
    }
    return false;
  }

  void supressScreenSaver() {
    //handled by event filter above
  }
#else
  #define None XNone
  #define Window XWindow
  #include <X11/extensions/XTest.h>
  #undef None
  #undef Window

  //POSIX-compatible (Linux, BSD, etc.)
  char* userpath(char *path) {
    *path = 0;
    struct passwd *userinfo = getpwuid(getuid());
    if(userinfo) strcpy(path, userinfo->pw_dir);
    return path;
  }

  char *getcwd(char *path) {
    return getcwd(path, PATH_MAX);
  }

  #define mkdir(path) (mkdir)(path, 0755)

  void initargs(int &argc, char **&argv) {
  }

  void supressScreenSaver() {
    static clock_t delta_x = 0, delta_y = 0;

    delta_y = clock();
    if(delta_y - delta_x < CLOCKS_PER_SEC * 20) return;
    delta_x = delta_y;

    //XSetScreenSaver(timeout = 0) does not work
    //XResetScreenSaver() does not work
    //XScreenSaverSuspend() does not work
    //DPMSDisable() does not work
    //XSendEvent(KeyPressMask) does not work
    //use XTest extension to send fake keypress every ~20 seconds.
    //keycode of 255 does not map to any actual key,
    //but it will block screensaver and power management.
    Display *display = XOpenDisplay(0);
    XTestFakeKeyEvent(display, 255, True,  0);
    XTestFakeKeyEvent(display, 255, False, 0);
  }
#endif
