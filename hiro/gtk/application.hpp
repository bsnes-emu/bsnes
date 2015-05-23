#if defined(Hiro_Application)

namespace hiro {

struct pApplication {
  #if defined(PLATFORM_XORG)
  static XlibDisplay* display;
  #endif

  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

}

#endif
