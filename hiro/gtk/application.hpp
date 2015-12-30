#if defined(Hiro_Application)

namespace hiro {

struct pApplication {
  #if defined(DISPLAY_XORG)
  static XlibDisplay* display;
  #endif

  static auto run() -> void;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;

  static auto initialize() -> void;
};

}

#endif
