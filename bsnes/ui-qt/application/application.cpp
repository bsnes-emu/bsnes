#include "application.moc"
VideoDisplay display;
Application application;

#include "init.cpp"

VideoDisplay::VideoDisplay() {
  outputWidth = 0;
  outputHeight = 0;

  cropLeft = 0;
  cropTop = 0;
  cropRight = 0;
  cropBottom = 0;
}

void Application::initPaths(const char *basename) {
  char temp[PATH_MAX];

  if(realpath(basename, temp)) {
    //remove program name
    strtr(temp, "\\", "/");
    for(signed i = strlen(temp) - 1; i >= 0; i--) {
      if(temp[i] == '/') {
        temp[i] = 0;
        break;
      }
    }

    if(strend(temp, "/") == false) strcat(temp, "/");
    config().path.base = temp;
  } else {
    config().path.base = "";
  }

  if(userpath(temp)) {
    strtr(temp, "\\", "/");
    if(strend(temp, "/") == false) strcat(temp, "/");
    config().path.user = temp;
  } else {
    config().path.user = "";
  }

  char cwd[PATH_MAX];
  config().path.startup = getcwd(cwd);
}

void Application::locateFile(string &filename, bool createDataDirectory) {
  //first, check if file exists in executable directory (single-user mode)
  string temp = string() << config().path.base << filename;

  if(file::exists(temp) == false) {
    //if not, use user data path (multi-user mode)
    temp = config().path.user;
    temp << ".bsnes";
    if(createDataDirectory) mkdir(temp, 0755);  //ensure directory exists
    temp << "/" << filename;
  }

  filename = temp;
}

int Application::main(int &argc, char **argv) {
  app = new App(argc, argv);
  #if !defined(PLATFORM_WIN)
  app->setWindowIcon(QIcon(":/bsnes.png"));
  #else
  //Windows port uses 256x256 icon from resource file
  CoInitialize(0);
  utf8_args(argc, argv);
  #endif

  initPaths(argv[0]);
  locateFile(configFilename = "bsnes.cfg", true);
  locateFile(styleSheetFilename = "style.qss", false);

  string customStylesheet;
  if(customStylesheet.readfile(styleSheetFilename) == true) {
    app->setStyleSheet((const char*)customStylesheet);
  } else {
    app->setStyleSheet(defaultStylesheet);
  }

  config().load(configFilename);
  mapper().bind();
  init();
  SNES::system.init(&interface);
  mainWindow->system_loadSpecial_superGameBoy->setVisible(SNES::supergameboy.opened());

  if(argc == 2) {
    //if valid file was specified on the command-line, attempt to load it now
    cartridge.loadNormal(argv[1]);
  }

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(run()));
  timer->start(0);
  app->exec();

  //QbWindow::close() saves window geometry for next run
  for(unsigned i = 0; i < windowList.size(); i++) {
    windowList[i]->close();
  }

  cartridge.unload();
  config().save(configFilename);
  return 0;
}

void Application::run() {
  if(terminate == true) {
    timer->stop();
    app->quit();
    return;
  }

  QApplication::processEvents();
  utility.updateSystemState();
  mapper().poll();

  if(config().input.focusPolicy == Configuration::Input::FocusPolicyPauseEmulation) {
    bool active = mainWindow->isActive();
    if(!autopause && !active) {
      autopause = true;
      audio.clear();
    } else if(autopause && active) {
      autopause = false;
    }
  } else {
    autopause = false;
  }

  if(SNES::cartridge.loaded() && !pause && !autopause && (!debug || debugrun)) {
    SNES::system.run();
    #if defined(DEBUGGER)
    if(SNES::debugger.break_event != SNES::Debugger::BreakEvent::None) {
      debug = true;
      debugrun = false;
      debugger->synchronize();
      debugger->event();
      SNES::debugger.break_event = SNES::Debugger::BreakEvent::None;
    }
    #endif
  } else {
    usleep(20 * 1000);
  }

  clock_t currentTime = clock();
  autosaveTime += currentTime - clockTime;
  screensaverTime += currentTime - clockTime;
  clockTime = currentTime;

  if(autosaveTime >= CLOCKS_PER_SEC * 60) {
    //auto-save RAM once per minute in case of emulator crash
    autosaveTime = 0;
    if(config().system.autoSaveMemory == true) cartridge.saveMemory();
  }

  if(screensaverTime >= CLOCKS_PER_SEC * 30) {
    //supress screen saver every 30 seconds so it will not trigger during gameplay
    screensaverTime = 0;
    supressScreenSaver();
  }
}

Application::Application() : timer(0) {
  terminate = false;
  power     = false;
  pause     = false;
  autopause = false;
  debug     = false;
  debugrun  = false;

  clockTime       = clock();
  autosaveTime    = 0;
  screensaverTime = 0;
}

Application::~Application() {
  delete timer;

  //deleting (QApplication)app will segfault the application upon exit
  //delete app;
}
