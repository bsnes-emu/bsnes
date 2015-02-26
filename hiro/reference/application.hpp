namespace phoenix {

struct pApplication {
  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

}
