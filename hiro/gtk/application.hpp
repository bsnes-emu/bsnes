namespace hiro {

struct pApplication {
  static XlibDisplay* display;

  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

}
