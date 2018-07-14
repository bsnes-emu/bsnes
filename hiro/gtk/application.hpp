#if defined(Hiro_Application)

namespace hiro {

struct pApplication {
  static auto run() -> void;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;

  static auto initialize() -> void;

  static vector<pWindow*> windows;

  #if defined(DISPLAY_XORG)
  static XlibDisplay* display;
  #endif
};

}

#endif
