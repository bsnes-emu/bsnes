#include "base.hpp"

Application *application = 0;
nall::DSP dspaudio;

//allow files to exist in the same folder as binary;
//otherwise default to home folder
string Application::path(const string &filename) {
  string result = { basepath, filename };
  if(file::exists(result)) return result;
  return { userpath, filename };
}

void Application::run() {
  inputManager->scan();

  autopause = (mainWindow->focused() == false && config->input.focusPolicy == 2);
  utility->updateStatus();

  if(interface->cartridgeLoaded() == false || pause || autopause) {
    audio.clear();
    usleep(20 * 1000);
    return;
  }

  interface->run();
}

Application::Application(int argc, char **argv) {
  title = "bsnes v084.06";

  application = this;
  quit = false;
  pause = false;
  autopause = false;

  {
    char path[PATH_MAX];
    auto unused = ::realpath(argv[0], path);
    basepath = dir(path);
    unused = ::userpath(path);
    userpath = path;
    if(Intrinsics::platform() == Intrinsics::Platform::Windows) {
      userpath.append("bsnes/");
    } else {
      userpath.append(".config/bsnes/");
    }
    mkdir(userpath, 0755);
  }

  config = new Config;
  interface = new Interface;
  inputManager = new InputManager;
  utility = new Utility;

  string fontFamily = Intrinsics::platform() == Intrinsics::Platform::Windows ? "Tahoma, " : "Sans, ";
  normalFont = { fontFamily, "8" };
  boldFont = { fontFamily, "8, Bold" };
  titleFont = { fontFamily, "16, Bold" };

  compositionEnable = compositor::enabled();
  if(config->video.compositionMode == 2) compositor::enable(false);

  windowManager = new WindowManager;
  mainWindow = new MainWindow;
  fileBrowser = new FileBrowser;
  slotLoader = new SlotLoader;
  dipSwitches = new DipSwitches;
  settingsWindow = new SettingsWindow;
  cheatDatabase = new CheatDatabase;
  cheatEditor = new CheatEditor;
  stateManager = new StateManager;
  windowManager->loadGeometry();

  utility->setMode(Interface::Mode::None);
  mainWindow->setVisible();

  video.driver(config->video.driver);
  video.set(Video::Handle, mainWindow->viewport.handle());
  video.set(Video::Synchronize, config->video.synchronize);
  video.set(Video::Depth, config->video.depth);
  if(video.init() == false) {
    MessageWindow::critical(*mainWindow, { "Failed to initialize ", config->video.driver, " video driver." });
    video.driver("None");
    video.init();
  }
  utility->bindVideoFilter();
  utility->bindVideoShader();

  audio.driver(config->audio.driver);
  audio.set(Audio::Handle, mainWindow->viewport.handle());
  audio.set(Audio::Synchronize, config->audio.synchronize);
  audio.set(Audio::Latency, config->audio.latency);
  audio.set(Audio::Frequency, config->audio.frequency);
  if(audio.init() == false) {
    MessageWindow::critical(*mainWindow, { "Failed to initialize ", config->audio.driver, " audio driver." });
    audio.driver("None");
    audio.init();
  }

  dspaudio.setPrecision(16);
  dspaudio.setVolume(config->audio.mute == false ? (double)config->audio.volume / 100.0 : 0.0);
  dspaudio.setBalance(0.0);
  dspaudio.setResampler(DSP::ResampleEngine::Sinc);
  dspaudio.setResamplerFrequency(config->audio.frequency);

  input.driver(config->input.driver);
  input.set(Input::Handle, mainWindow->viewport.handle());
  if(input.init() == false) {
    MessageWindow::critical(*mainWindow, { "Failed to initialize ", config->input.driver, " input driver." });
    input.driver("None");
    input.init();
  }

  if(config->video.startFullScreen) utility->toggleFullScreen();
  if(argc == 2) interface->loadCartridge(argv[1]);

  while(quit == false) {
    OS::processEvents();
    Application::run();
  }

  interface->unloadCartridge();
  windowManager->saveGeometry();
  if(compositionEnable) compositor::enable(true);
}

Application::~Application() {
  delete stateManager;
  delete cheatEditor;
  delete cheatDatabase;
  delete settingsWindow;
  delete dipSwitches;
  delete slotLoader;
  delete fileBrowser;
  delete mainWindow;
  delete windowManager;
  delete utility;
  delete inputManager;
  delete interface;
  delete config;
}

int main(int argc, char **argv) {
  new Application(argc, argv);
  delete application;
  return 0;
}
