class Application : public QObject {
  Q_OBJECT

public:
  class App : public QApplication {
  public:
    #if defined(PLATFORM_WIN)
    bool winEventFilter(MSG *msg, long *result);
    #endif

    App(int argc, char **argv) : QApplication(argc, argv) {}
  } *app;

  QTimer *timer;

  bool terminate;  //set to true to terminate main() loop and exit emulator
  bool power;
  bool pause;
  bool autopause;

  clock_t clockTime;
  clock_t autosaveTime;
  clock_t screensaverTime;

  string configFilename;
  string styleSheetFilename;

  int main(int argc, char **argv);
  void processEvents();
  void locateFile(string &filename, bool createDataDirectory = false);
  void initPaths(const char *basename);
  void init();

  Application();
  ~Application();

public slots:
  void run();
} application;

