#if defined(Hiro_Application)

namespace hiro {

struct pApplication {
  static auto modal() -> bool;
  static auto run() -> void;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;
  static auto setScreenSaver(bool screenSaver) -> void;

  static auto initialize() -> void;
  static auto synchronize() -> void;

  struct State {
    #if defined(DISPLAY_XORG)
    XlibDisplay* display = nullptr;
    XlibWindow screenSaverWindow = 0;
    bool screenSaverXDG = false;
    #endif
  };

  static auto state() -> State&;
};

static QApplication* qtApplication = nullptr;

}

#endif
