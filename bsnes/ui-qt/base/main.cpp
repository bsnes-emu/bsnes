#include "main.moc"
MainWindow *mainWindow;

MainWindow::MainWindow() {
  setObjectName("main-window");
  setWindowTitle(string() << SNES::Info::Name << " v" << SNES::Info::Version);
  setCloseOnEscape(false);
  setGeometryString(&config().geometry.mainWindow);
  application.windowList.append(this);

  //menu bar
  #if defined(PLATFORM_OSX)
  menuBar = new QMenuBar(0);
  #else
  menuBar = new QMenuBar;
  #endif

  system = menuBar->addMenu("&System");

  system_load = system->addAction("Load &Cartridge ...");

  system_loadSpecial = system->addMenu("Load &Special");

  system_loadSpecial_bsxSlotted = system_loadSpecial->addAction("Load BS-X &Slotted Cartridge ...");

  system_loadSpecial_bsx = system_loadSpecial->addAction("Load &BS-X Cartridge ...");

  system_loadSpecial_sufamiTurbo = system_loadSpecial->addAction("Load Sufami &Turbo Cartridge ...");

  system_loadSpecial_superGameBoy = system_loadSpecial->addAction("Load Super &Game Boy Cartridge ...");

  system->addSeparator();

  system->addAction(system_power = new CheckAction("&Power", 0));

  system_reset = system->addAction("&Reset");

  system->addSeparator();

  system_port1 = system->addMenu("Controller Port &1");
  system_port1->addAction(system_port1_none = new RadioAction("&None", 0));
  system_port1->addAction(system_port1_gamepad = new RadioAction("&Gamepad", 0));
  system_port1->addAction(system_port1_asciipad = new RadioAction("&asciiPad", 0));
  system_port1->addAction(system_port1_multitap = new RadioAction("&Multitap", 0));
  system_port1->addAction(system_port1_mouse = new RadioAction("&Mouse", 0));

  system_port2 = system->addMenu("Controller Port &2");
  system_port2->addAction(system_port2_none = new RadioAction("&None", 0));
  system_port2->addAction(system_port2_gamepad = new RadioAction("&Gamepad", 0));
  system_port2->addAction(system_port2_asciipad = new RadioAction("&asciiPad", 0));
  system_port2->addAction(system_port2_multitap = new RadioAction("&Multitap", 0));
  system_port2->addAction(system_port2_mouse = new RadioAction("&Mouse", 0));
  system_port2->addAction(system_port2_superscope = new RadioAction("&Super Scope", 0));
  system_port2->addAction(system_port2_justifier = new RadioAction("&Justifier", 0));
  system_port2->addAction(system_port2_justifiers = new RadioAction("Two &Justifiers", 0));

  #if !defined(PLATFORM_OSX)
  system->addSeparator();
  #endif

  system_exit = system->addAction("E&xit");
  system_exit->setMenuRole(QAction::QuitRole);

  settings = menuBar->addMenu("S&ettings");

  settings_videoMode = settings->addMenu("Video &Mode");

  settings_videoMode->addAction(settings_videoMode_1x = new RadioAction("Scale &1x", 0));

  settings_videoMode->addAction(settings_videoMode_2x = new RadioAction("Scale &2x", 0));

  settings_videoMode->addAction(settings_videoMode_3x = new RadioAction("Scale &3x", 0));

  settings_videoMode->addAction(settings_videoMode_4x = new RadioAction("Scale &4x", 0));

  settings_videoMode->addAction(settings_videoMode_5x = new RadioAction("Scale &5x", 0));

  settings_videoMode->addAction(settings_videoMode_max_normal = new RadioAction("Scale Max - &Normal", 0));

  settings_videoMode->addAction(settings_videoMode_max_wide = new RadioAction("Scale Max - &Wide", 0));

  settings_videoMode->addAction(settings_videoMode_max_wideZoom = new RadioAction("Scale Max - Wide &Zoom", 0));

  settings_videoMode->addSeparator();

  settings_videoMode->addAction(settings_videoMode_correctAspectRatio = new CheckAction("Correct &Aspect Ratio", 0));

  settings_videoMode->addSeparator();

  settings_videoMode->addAction(settings_videoMode_ntsc = new RadioAction("&NTSC", 0));
  settings_videoMode->addAction(settings_videoMode_pal = new RadioAction("&PAL", 0));

  if(filter.opened()) {
    settings_videoFilter = settings->addMenu("Video &Filter");

    settings_videoFilter_configure = settings_videoFilter->addAction("&Configure Active Filter ...");
    settings_videoFilter->addSeparator();

    settings_videoFilter->addAction(settings_videoFilter_none = new RadioAction("&None", 0));
    settings_videoFilter_list.append(settings_videoFilter_none);

    lstring filterlist;
    filterlist.split(";", filter.dl_supported());
    for(unsigned i = 0; i < filterlist.size(); i++) {
      RadioAction *action = new RadioAction(filterlist[i], 0);
      settings_videoFilter->addAction(action);
      settings_videoFilter_list.append(action);
    }
  }

  settings->addAction(settings_smoothVideo = new CheckAction("&Smooth Video Output", 0));

  settings->addSeparator();

  settings->addAction(settings_muteAudio = new CheckAction("&Mute Audio Output", 0));

  settings->addSeparator();

  settings_emulationSpeed = settings->addMenu("Emulation &Speed");

  settings_emulationSpeed->addAction(settings_emulationSpeed_slowest = new RadioAction("Slowest", 0));

  settings_emulationSpeed->addAction(settings_emulationSpeed_slow = new RadioAction("Slow", 0));

  settings_emulationSpeed->addAction(settings_emulationSpeed_normal = new RadioAction("Normal", 0));

  settings_emulationSpeed->addAction(settings_emulationSpeed_fast = new RadioAction("Fast", 0));

  settings_emulationSpeed->addAction(settings_emulationSpeed_fastest = new RadioAction("Fastest", 0));

  settings_emulationSpeed->addSeparator();

  settings_emulationSpeed->addAction(settings_emulationSpeed_syncVideo = new CheckAction("Sync &Video", 0));

  settings_emulationSpeed->addAction(settings_emulationSpeed_syncAudio = new CheckAction("Sync &Audio", 0));

  settings_configuration = settings->addAction("&Configuration ...");
  settings_configuration->setMenuRole(QAction::PreferencesRole);

  tools = menuBar->addMenu("&Tools");

  tools_movies = tools->addMenu("&Movies");

  tools_movies_play = tools_movies->addAction("Play Movie ...");

  tools_movies_stop = tools_movies->addAction("Stop");

  tools_movies_recordFromPowerOn = tools_movies->addAction("Record Movie (and restart system)");

  tools_movies_recordFromHere = tools_movies->addAction("Record Movie (starting from here)");

  tools_captureScreenshot = tools->addAction("&Capture Screenshot");

  #if 0
  //this will crash on Qt 4.6.0/Windows, because QObject::sender() returns a non-QObject*, non-null pointer
  //since we don't know what other Qt toolkits have this bug, it's safer to just disable the feature by default
  tools->addSeparator();

  tools_loadState = tools->addMenu("&Load Quick State");
  for(unsigned i = 0; i < 10; i++) {
    QAction *loadAction = new QAction(string("Slot ", i + 1), 0);
    loadAction->setData(i);
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadState()));
    tools_loadState->addAction(loadAction);
  }

  tools_saveState = tools->addMenu("&Save Quick State");
  for(unsigned i = 0; i < 10; i++) {
    QAction *saveAction = new QAction(string("Slot ", i + 1), 0);
    saveAction->setData(i);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveState()));
    tools_saveState->addAction(saveAction);
  }
  #endif

  tools->addSeparator();

  tools_cheatEditor = tools->addAction("Cheat &Editor ...");

  tools_cheatFinder = tools->addAction("Cheat &Finder ...");

  tools_stateManager = tools->addAction("&State Manager ...");

  tools_effectToggle = tools->addAction("Effect &Toggle ...");
  #if !defined(PROFILE_COMPATIBILITY) && !defined(PROFILE_PERFORMANCE)
  tools_effectToggle->setVisible(false);
  #endif

  tools_debugger = tools->addAction("&Debugger ...");
  #if !defined(DEBUGGER)
  tools_debugger->setVisible(false);
  #endif

  help = menuBar->addMenu("&Help");

  help_documentation = help->addAction("&Documentation ...");

  help_license = help->addAction("&License ...");

  #if !defined(PLATFORM_OSX)
  help->addSeparator();
  #endif

  help_about = help->addAction("&About ...");
  help_about->setMenuRole(QAction::AboutRole);

  //canvas
  canvasContainer = new CanvasObject;
  canvasContainer->setAcceptDrops(true); {
    canvasContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvasContainer->setObjectName("backdrop");

    canvasLayout = new QVBoxLayout; {
      canvasLayout->setMargin(0);
      canvasLayout->setAlignment(Qt::AlignCenter);

      canvas = new CanvasWidget;
      canvas->setAcceptDrops(true);
      canvas->setFocusPolicy(Qt::StrongFocus);
      canvas->setAttribute(Qt::WA_PaintOnScreen, true);  //disable Qt painting on focus / resize

      QPalette palette;
      palette.setColor(QPalette::Window, QColor(0, 0, 0));

      canvas->setPalette(palette);
      canvas->setAutoFillBackground(true);
    }
    canvasLayout->addWidget(canvas);
  }
  canvasContainer->setLayout(canvasLayout);

  //status bar
  statusBar = new QStatusBar;
  statusBar->setSizeGripEnabled(false);
  statusBar->showMessage("");
  systemState = new QLabel;
  statusBar->addPermanentWidget(systemState);

  //layout
  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);
  #if !defined(PLATFORM_OSX)
  layout->addWidget(menuBar);
  #endif
  layout->addWidget(canvasContainer);
  layout->addWidget(statusBar);
  setLayout(layout);

  //slots
  connect(system_load, SIGNAL(triggered()), this, SLOT(loadCartridge()));
  connect(system_loadSpecial_bsxSlotted, SIGNAL(triggered()), this, SLOT(loadBsxSlottedCartridge()));
  connect(system_loadSpecial_bsx, SIGNAL(triggered()), this, SLOT(loadBsxCartridge()));
  connect(system_loadSpecial_sufamiTurbo, SIGNAL(triggered()), this, SLOT(loadSufamiTurboCartridge()));
  connect(system_loadSpecial_superGameBoy, SIGNAL(triggered()), this, SLOT(loadSuperGameBoyCartridge()));
  connect(system_power, SIGNAL(triggered()), this, SLOT(power()));
  connect(system_reset, SIGNAL(triggered()), this, SLOT(reset()));
  connect(system_port1_none, SIGNAL(triggered()), this, SLOT(setPort1None()));
  connect(system_port1_gamepad, SIGNAL(triggered()), this, SLOT(setPort1Gamepad()));
  connect(system_port1_asciipad, SIGNAL(triggered()), this, SLOT(setPort1Asciipad()));
  connect(system_port1_multitap, SIGNAL(triggered()), this, SLOT(setPort1Multitap()));
  connect(system_port1_mouse, SIGNAL(triggered()), this, SLOT(setPort1Mouse()));
  connect(system_port2_none, SIGNAL(triggered()), this, SLOT(setPort2None()));
  connect(system_port2_gamepad, SIGNAL(triggered()), this, SLOT(setPort2Gamepad()));
  connect(system_port2_asciipad, SIGNAL(triggered()), this, SLOT(setPort2Asciipad()));
  connect(system_port2_multitap, SIGNAL(triggered()), this, SLOT(setPort2Multitap()));
  connect(system_port2_mouse, SIGNAL(triggered()), this, SLOT(setPort2Mouse()));
  connect(system_port2_superscope, SIGNAL(triggered()), this, SLOT(setPort2SuperScope()));
  connect(system_port2_justifier, SIGNAL(triggered()), this, SLOT(setPort2Justifier()));
  connect(system_port2_justifiers, SIGNAL(triggered()), this, SLOT(setPort2Justifiers()));
  connect(system_exit, SIGNAL(triggered()), this, SLOT(quit()));
  connect(settings_videoMode_1x, SIGNAL(triggered()), this, SLOT(setVideoMode1x()));
  connect(settings_videoMode_2x, SIGNAL(triggered()), this, SLOT(setVideoMode2x()));
  connect(settings_videoMode_3x, SIGNAL(triggered()), this, SLOT(setVideoMode3x()));
  connect(settings_videoMode_4x, SIGNAL(triggered()), this, SLOT(setVideoMode4x()));
  connect(settings_videoMode_5x, SIGNAL(triggered()), this, SLOT(setVideoMode5x()));
  connect(settings_videoMode_max_normal, SIGNAL(triggered()), this, SLOT(setVideoModeMaxNormal()));
  connect(settings_videoMode_max_wide, SIGNAL(triggered()), this, SLOT(setVideoModeMaxWide()));
  connect(settings_videoMode_max_wideZoom, SIGNAL(triggered()), this, SLOT(setVideoModeMaxWideZoom()));
  connect(settings_videoMode_correctAspectRatio, SIGNAL(triggered()), this, SLOT(toggleAspectCorrection()));
  connect(settings_videoMode_ntsc, SIGNAL(triggered()), this, SLOT(setVideoNtsc()));
  connect(settings_videoMode_pal, SIGNAL(triggered()), this, SLOT(setVideoPal()));
  if(filter.opened()) {
    connect(settings_videoFilter_configure, SIGNAL(triggered()), this, SLOT(configureFilter()));
    for(unsigned i = 0; i < settings_videoFilter_list.size(); i++) {
      connect(settings_videoFilter_list[i], SIGNAL(triggered()), this, SLOT(setFilter()));
    }
  }
  connect(settings_smoothVideo, SIGNAL(triggered()), this, SLOT(toggleSmoothVideo()));
  connect(settings_muteAudio, SIGNAL(triggered()), this, SLOT(muteAudio()));
  connect(settings_emulationSpeed_slowest, SIGNAL(triggered()), this, SLOT(setSpeedSlowest()));
  connect(settings_emulationSpeed_slow, SIGNAL(triggered()), this, SLOT(setSpeedSlow()));
  connect(settings_emulationSpeed_normal, SIGNAL(triggered()), this, SLOT(setSpeedNormal()));
  connect(settings_emulationSpeed_fast, SIGNAL(triggered()), this, SLOT(setSpeedFast()));
  connect(settings_emulationSpeed_fastest, SIGNAL(triggered()), this, SLOT(setSpeedFastest()));
  connect(settings_emulationSpeed_syncVideo, SIGNAL(triggered()), this, SLOT(syncVideo()));
  connect(settings_emulationSpeed_syncAudio, SIGNAL(triggered()), this, SLOT(syncAudio()));
  connect(settings_configuration, SIGNAL(triggered()), this, SLOT(showConfigWindow()));
  connect(tools_movies_play, SIGNAL(triggered()), this, SLOT(playMovie()));
  connect(tools_movies_stop, SIGNAL(triggered()), this, SLOT(stopMovie()));
  connect(tools_movies_recordFromPowerOn, SIGNAL(triggered()), this, SLOT(recordMovieFromPowerOn()));
  connect(tools_movies_recordFromHere, SIGNAL(triggered()), this, SLOT(recordMovieFromHere()));
  connect(tools_captureScreenshot, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
  connect(tools_cheatEditor, SIGNAL(triggered()), this, SLOT(showCheatEditor()));
  connect(tools_cheatFinder, SIGNAL(triggered()), this, SLOT(showCheatFinder()));
  connect(tools_stateManager, SIGNAL(triggered()), this, SLOT(showStateManager()));
  connect(tools_effectToggle, SIGNAL(triggered()), this, SLOT(showEffectToggle()));
  connect(tools_debugger, SIGNAL(triggered()), this, SLOT(showDebugger()));
  connect(help_documentation, SIGNAL(triggered()), this, SLOT(showDocumentation()));
  connect(help_license, SIGNAL(triggered()), this, SLOT(showLicense()));
  connect(help_about, SIGNAL(triggered()), this, SLOT(showAbout()));

  syncUi();
}

void MainWindow::syncUi() {
  system_power->setEnabled(SNES::cartridge.loaded());
  system_power->setChecked(application.power == true);
  system_power->setEnabled(SNES::cartridge.loaded());
  system_reset->setEnabled(SNES::cartridge.loaded() && application.power);

  system_port1_none->setChecked      (config().input.port1 == ControllerPort1::None);
  system_port1_gamepad->setChecked   (config().input.port1 == ControllerPort1::Gamepad);
  system_port1_asciipad->setChecked  (config().input.port1 == ControllerPort1::Asciipad);
  system_port1_multitap->setChecked  (config().input.port1 == ControllerPort1::Multitap);
  system_port1_mouse->setChecked     (config().input.port1 == ControllerPort1::Mouse);

  system_port2_none->setChecked      (config().input.port2 == ControllerPort2::None);
  system_port2_gamepad->setChecked   (config().input.port2 == ControllerPort2::Gamepad);
  system_port2_asciipad->setChecked  (config().input.port2 == ControllerPort2::Asciipad);
  system_port2_multitap->setChecked  (config().input.port2 == ControllerPort2::Multitap);
  system_port2_mouse->setChecked     (config().input.port2 == ControllerPort2::Mouse);
  system_port2_superscope->setChecked(config().input.port2 == ControllerPort2::SuperScope);
  system_port2_justifier->setChecked (config().input.port2 == ControllerPort2::Justifier);
  system_port2_justifiers->setChecked(config().input.port2 == ControllerPort2::Justifiers);

  settings_videoMode_1x->setChecked        (config().video.context->multiplier == 1);
  settings_videoMode_2x->setChecked        (config().video.context->multiplier == 2);
  settings_videoMode_3x->setChecked        (config().video.context->multiplier == 3);
  settings_videoMode_4x->setChecked        (config().video.context->multiplier == 4);
  settings_videoMode_5x->setChecked        (config().video.context->multiplier == 5);
  settings_videoMode_max_normal->setChecked(config().video.context->multiplier == 6);
  settings_videoMode_max_wide->setChecked  (config().video.context->multiplier == 7);
  settings_videoMode_max_wideZoom->setChecked (config().video.context->multiplier == 8);

  settings_videoMode_max_normal->setVisible(config().video.isFullscreen == true);
  settings_videoMode_max_wide->setVisible  (config().video.isFullscreen == true);
  settings_videoMode_max_wideZoom->setVisible (config().video.isFullscreen == true);

  settings_videoMode_correctAspectRatio->setChecked(config().video.context->correctAspectRatio);
  settings_videoMode_ntsc->setChecked(config().video.context->region == 0);
  settings_videoMode_pal->setChecked (config().video.context->region == 1);

  if(filter.opened()) {
    //only enable configuration option if the active filter supports it ...
    settings_videoFilter_configure->setEnabled(filter.settings());

    for(unsigned i = 0; i < settings_videoFilter_list.size(); i++) {
      settings_videoFilter_list[i]->setChecked(config().video.context->swFilter == i);
    }
  }

  settings_smoothVideo->setChecked(config().video.context->hwFilter == 1);
  settings_muteAudio->setChecked(config().audio.mute);

  settings_emulationSpeed_slowest->setChecked(config().system.speed == 0);
  settings_emulationSpeed_slow->setChecked   (config().system.speed == 1);
  settings_emulationSpeed_normal->setChecked (config().system.speed == 2);
  settings_emulationSpeed_fast->setChecked   (config().system.speed == 3);
  settings_emulationSpeed_fastest->setChecked(config().system.speed == 4);

  settings_emulationSpeed_syncVideo->setChecked(config().video.synchronize);
  settings_emulationSpeed_syncAudio->setChecked(config().audio.synchronize);

  //movies contian save states to synchronize playback to recorded input
  tools_movies->setEnabled(SNES::cartridge.loaded() && cartridge.saveStatesSupported());
  if(tools_movies->isEnabled()) {
    tools_movies_play->setEnabled(movie.state == Movie::Inactive);
    tools_movies_stop->setEnabled(movie.state != Movie::Inactive);
    tools_movies_recordFromPowerOn->setEnabled(movie.state == Movie::Inactive);
    tools_movies_recordFromHere->setEnabled(movie.state == Movie::Inactive);
  }
}

bool MainWindow::isActive() {
  return isActiveWindow() && !isMinimized();
}

void MainWindow::loadCartridge() {
  fileBrowser->setWindowTitle("Load Cartridge");
  fileBrowser->loadCartridge(FileBrowser::LoadDirect);
}

void MainWindow::loadBsxSlottedCartridge() {
  loaderWindow->loadBsxSlottedCartridge("", "");
}

void MainWindow::loadBsxCartridge() {
  loaderWindow->loadBsxCartridge(config().path.bsx, "");
}

void MainWindow::loadSufamiTurboCartridge() {
  loaderWindow->loadSufamiTurboCartridge(config().path.st, "", "");
}

void MainWindow::loadSuperGameBoyCartridge() {
  loaderWindow->loadSuperGameBoyCartridge(config().path.sgb, "");
}

void MainWindow::power() {
  system_power->toggleChecked();
  if(system_power->isChecked()) {
    utility.modifySystemState(Utility::PowerOn);
  } else {
    utility.modifySystemState(Utility::PowerOff);
  }
}

void MainWindow::reset() {
  utility.modifySystemState(Utility::Reset);
}

void MainWindow::setPort1None() {
  config().input.port1 = ControllerPort1::None;
  SNES::config.controller_port1 = SNES::Input::Device::None;
  utility.updateControllers();
}

void MainWindow::setPort1Gamepad() {
  config().input.port1 = ControllerPort1::Gamepad;
  SNES::config.controller_port1 = SNES::Input::Device::Joypad;
  utility.updateControllers();
}

void MainWindow::setPort1Asciipad() {
  config().input.port1 = ControllerPort1::Asciipad;
  SNES::config.controller_port1 = SNES::Input::Device::Joypad;
  utility.updateControllers();
}

void MainWindow::setPort1Multitap() {
  config().input.port1 = ControllerPort1::Multitap;
  SNES::config.controller_port1 = SNES::Input::Device::Multitap;
  utility.updateControllers();
}

void MainWindow::setPort1Mouse() {
  config().input.port1 = ControllerPort1::Mouse;
  SNES::config.controller_port1 = SNES::Input::Device::Mouse;
  utility.updateControllers();
}

void MainWindow::setPort2None() {
  config().input.port2 = ControllerPort2::None;
  SNES::config.controller_port2 = SNES::Input::Device::None;
  utility.updateControllers();
}

void MainWindow::setPort2Gamepad() {
  config().input.port2 = ControllerPort2::Gamepad;
  SNES::config.controller_port2 = SNES::Input::Device::Joypad;
  utility.updateControllers();
}

void MainWindow::setPort2Asciipad() {
  config().input.port2 = ControllerPort2::Asciipad;
  SNES::config.controller_port2 = SNES::Input::Device::Joypad;
  utility.updateControllers();
}

void MainWindow::setPort2Multitap() {
  config().input.port2 = ControllerPort2::Multitap;
  SNES::config.controller_port2 = SNES::Input::Device::Multitap;
  utility.updateControllers();
}

void MainWindow::setPort2Mouse() {
  config().input.port2 = ControllerPort2::Mouse;
  SNES::config.controller_port2 = SNES::Input::Device::Mouse;
  utility.updateControllers();
}

void MainWindow::setPort2SuperScope() {
  config().input.port2 = ControllerPort2::SuperScope;
  SNES::config.controller_port2 = SNES::Input::Device::SuperScope;
  utility.updateControllers();
}

void MainWindow::setPort2Justifier() {
  config().input.port2 = ControllerPort2::Justifier;
  SNES::config.controller_port2 = SNES::Input::Device::Justifier;
  utility.updateControllers();
}

void MainWindow::setPort2Justifiers() {
  config().input.port2 = ControllerPort2::Justifiers;
  SNES::config.controller_port2 = SNES::Input::Device::Justifiers;
  utility.updateControllers();
}

void MainWindow::quit() {
  hide();
  application.terminate = true;
}

void MainWindow::setVideoMode1x()          { utility.setScale(1); }
void MainWindow::setVideoMode2x()          { utility.setScale(2); }
void MainWindow::setVideoMode3x()          { utility.setScale(3); }
void MainWindow::setVideoMode4x()          { utility.setScale(4); }
void MainWindow::setVideoMode5x()          { utility.setScale(5); }
void MainWindow::setVideoModeMaxNormal()   { utility.setScale(6); }
void MainWindow::setVideoModeMaxWide()     { utility.setScale(7); }
void MainWindow::setVideoModeMaxWideZoom() { utility.setScale(8); }

void MainWindow::toggleAspectCorrection() { utility.toggleAspectCorrection(); }

void MainWindow::setVideoNtsc() { utility.setNtscMode(); }
void MainWindow::setVideoPal()  { utility.setPalMode(); }

void MainWindow::toggleSmoothVideo() { utility.toggleSmoothVideoOutput(); }

void MainWindow::configureFilter() {
  QWidget *widget = filter.settings();
  if(widget) {
    widget->show();
    widget->activateWindow();
    widget->raise();
  }
}

void MainWindow::setFilter() {
  for(unsigned i = 0; i < settings_videoFilter_list.size(); i++) {
    if(sender() == settings_videoFilter_list[i]) {
      config().video.context->swFilter = i;
      utility.updateSoftwareFilter();
      syncUi();
      return;
    }
  }
}

void MainWindow::muteAudio() {
  settings_muteAudio->toggleChecked();
  config().audio.mute = settings_muteAudio->isChecked();
}

void MainWindow::setSpeedSlowest() { config().system.speed = 0; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedSlow()    { config().system.speed = 1; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedNormal()  { config().system.speed = 2; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedFast()    { config().system.speed = 3; utility.updateEmulationSpeed(); syncUi(); }
void MainWindow::setSpeedFastest() { config().system.speed = 4; utility.updateEmulationSpeed(); syncUi(); }

void MainWindow::syncVideo() { utility.toggleSynchronizeVideo(); }
void MainWindow::syncAudio() { utility.toggleSynchronizeAudio(); }

void MainWindow::showConfigWindow() { settingsWindow->show(); }

void MainWindow::playMovie() {
  movie.chooseFile();
  syncUi();
}

void MainWindow::stopMovie() {
  movie.stop();
  syncUi();
}

void MainWindow::recordMovieFromPowerOn() {
  utility.modifySystemState(Utility::PowerCycle);
  movie.record();
  syncUi();
}

void MainWindow::recordMovieFromHere() {
  movie.record();
  syncUi();
}

void MainWindow::saveScreenshot() {
  //tell SNES::Interface to save a screenshot at the next video_refresh() event
  interface.saveScreenshot = true;
}

void MainWindow::loadState() {
  QAction *action = dynamic_cast<QAction*>(sender());
  if(action == 0) return;
  unsigned slot = action->data().toUInt();
  state.load(slot);
}

void MainWindow::saveState() {
  QAction *action = dynamic_cast<QAction*>(sender());
  if(action == 0) return;
  unsigned slot = action->data().toUInt();
  state.save(slot);
}

void MainWindow::showCheatEditor()  { toolsWindow->tab->setCurrentIndex(0); toolsWindow->show(); }
void MainWindow::showCheatFinder()  { toolsWindow->tab->setCurrentIndex(1); toolsWindow->show(); }
void MainWindow::showStateManager() { toolsWindow->tab->setCurrentIndex(2); toolsWindow->show(); }
void MainWindow::showEffectToggle() { toolsWindow->tab->setCurrentIndex(3); toolsWindow->show(); }

void MainWindow::showDebugger() {
  #if defined(DEBUGGER)
  debugger->show();
  #endif
}

void MainWindow::showDocumentation()  {
  QFile file(":/documentation.html");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    htmlViewerWindow->show("Usage Documentation", file.readAll().constData());
    file.close();
  }
}

void MainWindow::showLicense() {
  QFile file(":/license.html");
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    htmlViewerWindow->show("License Agreement", file.readAll().constData());
    file.close();
  }
}
void MainWindow::showAbout() {
  aboutWindow->show();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  Window::resizeEvent(event);
  QApplication::processEvents();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  Window::closeEvent(event);
  quit();
}

//============
//CanvasObject
//============

//implement drag-and-drop support:
//drag cartridge image onto main window canvas area to load

void CanvasObject::dragEnterEvent(QDragEnterEvent *event) {
  if(event->mimeData()->hasUrls()) {
    //do not accept multiple files at once
    if(event->mimeData()->urls().count() == 1) event->acceptProposedAction();
  }
}

void CanvasObject::dropEvent(QDropEvent *event) {
  if(event->mimeData()->hasUrls()) {
    QList<QUrl> list = event->mimeData()->urls();
    if(list.count() == 1) cartridge.loadNormal(list.at(0).toLocalFile().toUtf8().constData());
  }
}

//accept all key events for this widget to prevent system chime from playing on OS X
//key events are actually handled by Input class

void CanvasObject::keyPressEvent(QKeyEvent *event) {
}

void CanvasObject::keyReleaseEvent(QKeyEvent *event) {
}

//===========
//CanvasWidget
//============

//custom video render and mouse capture functionality

QPaintEngine* CanvasWidget::paintEngine() const {
  if(SNES::cartridge.loaded()) {
    video.refresh();
    return 0;
  }
  return QWidget::paintEngine();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
  //acquire exclusive mode access to mouse when video output widget is clicked
  //(will only acquire if cart is loaded, and mouse / lightgun is in use.)
  utility.acquireMouse();
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
  event->ignore();
}
