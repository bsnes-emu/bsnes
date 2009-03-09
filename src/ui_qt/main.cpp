#include "main.hpp"
#include "resource/resource.rcc"

class utf8 : public nall::string {
public:
  utf8& operator<<(const string &s) { string::operator<<(s); return *this; }
  utf8& operator<<(const char *s) { string::operator<<(s); return *this; }
  utf8& operator<<(int n) { string::operator<<(n); return *this; }
  utf8& operator<<(double d) { string::operator<<(d); return *this; }
  operator const QString() const { return QString::fromUtf8(*this); }
};

#include "platform.cpp"
#include "config.cpp"
#include "interface.cpp"
#include "ui.cpp"

#include "input/input.cpp"
#include "utility/utility.cpp"

const char defaultStylesheet[] =
  "QLabel.title {\n"
  "  background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 rgba(255, 0, 0, 48), stop: 1 rgba(0, 0, 0, 32));\n"
  "  font-weight: bold;\n"
  "  margin-bottom: 5px;\n"
  "  padding: 3px;\n"
  "}\n"
  "\n"
  "#backdrop {\n"
  "  background: #000000;\n"
  "}\n"
  "\n"
  "#about-window {\n"
  "  background: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 1, stop: 0 #555555, stop: 1 #aaaaaa);\n"
  "}\n"
  "";

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
    snes.config.path.base = temp;
  } else {
    snes.config.path.base = "";
  }

  if(userpath(temp)) {
    strtr(temp, "\\", "/");
    if(strend(temp, "/") == false) strcat(temp, "/");
    snes.config.path.user = temp;
  } else {
    snes.config.path.user = "";
  }

  char cwd[PATH_MAX];
  snes.config.path.current = getcwd(cwd);
}

void Application::locateFile(string &filename, bool createDataDirectory) {
  //first, check if file exists in executable directory (single-user mode)
  string temp = string() << snes.config.path.base << filename;

  if(file::exists(temp) == false) {
    //if not, use user data path (multi-user mode)
    temp = snes.config.path.user;
    temp << ".bsnes";
    if(createDataDirectory) mkdir(temp);  //ensure directory exists
    temp << "/" << filename;
  }

  filename = temp;
}

int Application::main(int argc, char **argv) {
  app = new QApplication(argc, argv);
  #if !defined(_WIN32)
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
  snes.init();

  if(argc == 2) {
    //if valid file was specified on the command-line, attempt to load it now
    utility.loadCartridge(argv[1]);
  }

  while(terminate == false) {
    processEvents();
    utility.updateSystemState();
    inputManager.refresh();

    if(config.input.focusPolicy == Configuration::Input::FocusPolicyPauseEmulation) {
      bool inactive = (winMain->window->isActiveWindow() == false);
      if(!autopause && inactive) {
        autopause = true;
        audio.clear();
      } else if(autopause && !inactive) {
        autopause = false;
      }
    } else {
      autopause = false;
    }

    if(cartridge.loaded() && !pause && !autopause) {
      snes.runtoframe();
    } else {
      usleep(20 * 1000);
    }
  }

  config.save(configFilename);
  return 0;
}

void Application::processEvents() {
  app->processEvents();
}

Application::Application() {
  terminate = false;
  power     = false;
  pause     = false;
  autopause = false;
}

Application::~Application() {
  //deleting (QApplication)app will segfault the application upon exit
  //delete app;
}

int main(int argc, char **argv) {
  return application.main(argc, argv);
}
