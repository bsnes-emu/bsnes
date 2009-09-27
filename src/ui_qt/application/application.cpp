#include "init.cpp"

const char* FileReader::direct_supported() {
  return "";
}

bool FileReader::direct_load(const char *filename, uint8_t **outdata, unsigned *outsize) {
  if(file::exists(filename) == false) return false;

  file fp;
  if(fp.open(filename, file::mode_read) == false) return false;

  unsigned size;
  uint8_t *data = new uint8_t[size = fp.size()];
  fp.read(data, size);
  fp.close();

  //remove copier header, if it exists
  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);

  *outdata = data;
  *outsize = size;
  return true;
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
    config.path.base = temp;
  } else {
    config.path.base = "";
  }

  if(userpath(temp)) {
    strtr(temp, "\\", "/");
    if(strend(temp, "/") == false) strcat(temp, "/");
    config.path.user = temp;
  } else {
    config.path.user = "";
  }

  char cwd[PATH_MAX];
  config.path.current = getcwd(cwd);
}

void Application::locateFile(string &filename, bool createDataDirectory) {
  //first, check if file exists in executable directory (single-user mode)
  string temp = string() << config.path.base << filename;

  if(file::exists(temp) == false) {
    //if not, use user data path (multi-user mode)
    temp = config.path.user;
    temp << ".bsnes";
    if(createDataDirectory) mkdir(temp);  //ensure directory exists
    temp << "/" << filename;
  }

  filename = temp;
}

int Application::main(int &argc, char **argv) {
  app = new App(argc, argv);
  #if !defined(PLATFORM_WIN)
  //Windows port uses 256x256 icon from resource file
  app->setWindowIcon(QIcon(":/bsnes.png"));
  #endif

  initargs(argc, argv);  //ensure argv[]s are in UTF-8 format
  initPaths(argv[0]);
  locateFile(configFilename = "bsnes.cfg", true);
  locateFile(styleSheetFilename = "style.qss", false);

  string customStylesheet;
  if(customStylesheet.readfile(styleSheetFilename) == true) {
    app->setStyleSheet((const char*)customStylesheet);
  } else {
    app->setStyleSheet(defaultStylesheet);
  }

  config.load(configFilename);
  init();
  SNES::system.init(&interface);

  if(argc == 2) {
    //if valid file was specified on the command-line, attempt to load it now
    utility.loadCartridge(argv[1]);
  }

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(run()));
  timer->start(0);
  app->exec();

  utility.unloadCartridge();
  config.save(configFilename);
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
  inputManager.refresh();

  if(config.input.focusPolicy == Configuration::Input::FocusPolicyPauseEmulation) {
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
    if(SNES::debugger.break_event != SNES::Debugger::None) {
      debug = true;
      debugrun = false;
      debugger->synchronize();
      debugger->event();
      SNES::debugger.break_event = SNES::Debugger::None;
    }
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
    if(config.system.autoSaveMemory == true) utility.saveMemory();
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
