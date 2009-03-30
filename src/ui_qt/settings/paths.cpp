void PathSettingsWindow::setup() {
  panel = new QWidget;

  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  title = new QLabel("Default Folder Paths");
  title->setProperty("class", "title");
  layout->addWidget(title);

  gameLabel = new QLabel("Games:");
  layout->addWidget(gameLabel);

  games = new QHBoxLayout; {
    games->setMargin(0);

    gamePath = new QLineEdit;
    gamePath->setReadOnly(true);
    games->addWidget(gamePath);

    gameSelect = new QPushButton("Select ...");
    games->addWidget(gameSelect);

    gameDefault = new QPushButton("Default");
    games->addWidget(gameDefault);
  }
  games->setSpacing(Style::WidgetSpacing);
  layout->addLayout(games);
  layout->addSpacing(Style::WidgetSpacing);

  saveLabel = new QLabel("Save RAM:");
  layout->addWidget(saveLabel);

  saves = new QHBoxLayout; {
    saves->setMargin(0);

    savePath = new QLineEdit;
    savePath->setReadOnly(true);
    saves->addWidget(savePath);

    saveSelect = new QPushButton("Select ...");
    saves->addWidget(saveSelect);

    saveDefault = new QPushButton("Default");
    saves->addWidget(saveDefault);
  }
  saves->setSpacing(Style::WidgetSpacing);
  layout->addLayout(saves);
  layout->addSpacing(Style::WidgetSpacing);

  patchLabel = new QLabel("UPS patches:");
  layout->addWidget(patchLabel);

  patches = new QHBoxLayout; {
    patches->setMargin(0);

    patchPath = new QLineEdit;
    patchPath->setReadOnly(true);
    patches->addWidget(patchPath);

    patchSelect = new QPushButton("Select ...");
    patches->addWidget(patchSelect);

    patchDefault = new QPushButton("Default");
    patches->addWidget(patchDefault);
  }
  patches->setSpacing(Style::WidgetSpacing);
  layout->addLayout(patches);
  layout->addSpacing(Style::WidgetSpacing);

  cheatLabel = new QLabel("Cheat codes:");
  layout->addWidget(cheatLabel);

  cheats = new QHBoxLayout; {
    cheats->setMargin(0);

    cheatPath = new QLineEdit;
    cheatPath->setReadOnly(true);
    cheats->addWidget(cheatPath);

    cheatSelect = new QPushButton("Select ...");
    cheats->addWidget(cheatSelect);

    cheatDefault = new QPushButton("Default");
    cheats->addWidget(cheatDefault);
  }
  cheats->setSpacing(Style::WidgetSpacing);
  layout->addLayout(cheats);
  layout->addSpacing(Style::WidgetSpacing);

  dataLabel = new QLabel("Export data:");
  layout->addWidget(dataLabel);

  data = new QHBoxLayout;
  data->setMargin(0);
  data->setSpacing(Style::WidgetSpacing); {
    dataPath = new QLineEdit;
    dataPath->setReadOnly(true);
    data->addWidget(dataPath);

    dataSelect = new QPushButton("Select ...");
    data->addWidget(dataSelect);

    dataDefault = new QPushButton("Default");
    data->addWidget(dataDefault);
  }
  layout->addLayout(data);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer);

  syncUi();

  panel->setLayout(layout);
  connect(gameSelect, SIGNAL(released()), this, SLOT(selectGamePath()));
  connect(gameDefault, SIGNAL(released()), this, SLOT(defaultGamePath()));
  connect(saveSelect, SIGNAL(released()), this, SLOT(selectSavePath()));
  connect(saveDefault, SIGNAL(released()), this, SLOT(defaultSavePath()));
  connect(patchSelect, SIGNAL(released()), this, SLOT(selectPatchPath()));
  connect(patchDefault, SIGNAL(released()), this, SLOT(defaultPatchPath()));
  connect(cheatSelect, SIGNAL(released()), this, SLOT(selectCheatPath()));
  connect(cheatDefault, SIGNAL(released()), this, SLOT(defaultCheatPath()));
  connect(dataSelect, SIGNAL(released()), this, SLOT(selectDataPath()));
  connect(dataDefault, SIGNAL(released()), this, SLOT(defaultDataPath()));
}

void PathSettingsWindow::syncUi() {
  gamePath->setText (snes.config.path.rom   == "" ? "<startup path>"               : (const char*)snes.config.path.rom);
  savePath->setText (snes.config.path.save  == "" ? "<same folder as loaded game>" : (const char*)snes.config.path.save);
  patchPath->setText(snes.config.path.patch == "" ? "<same folder as loaded game>" : (const char*)snes.config.path.patch);
  cheatPath->setText(snes.config.path.cheat == "" ? "<same folder as loaded game>" : (const char*)snes.config.path.cheat);
  dataPath->setText (snes.config.path.data  == "" ? "<same folder as loaded game>" : (const char*)snes.config.path.data);
}

void PathSettingsWindow::selectGamePath() {
  string path = utility.selectFolder("Default Game Path");
  if(path.length() > 0) {
    snes.config.path.rom = path;
    syncUi();
  }
}

void PathSettingsWindow::defaultGamePath() {
  snes.config.path.rom = "";
  syncUi();
}

void PathSettingsWindow::selectSavePath() {
  string path = utility.selectFolder("Default Save RAM Path");
  if(path.length() > 0) {
    snes.config.path.save = path;
    syncUi();
  }
}

void PathSettingsWindow::defaultSavePath() {
  snes.config.path.save = "";
  syncUi();
}

void PathSettingsWindow::selectPatchPath() {
  string path = utility.selectFolder("Default UPS Patch Path");
  if(path.length() > 0) {
    snes.config.path.patch = path;
    syncUi();
  }
}

void PathSettingsWindow::defaultPatchPath() {
  snes.config.path.patch = "";
  syncUi();
}

void PathSettingsWindow::selectCheatPath() {
  string path = utility.selectFolder("Default Cheat File Path");
  if(path.length() > 0) {
    snes.config.path.cheat = path;
    syncUi();
  }
}

void PathSettingsWindow::defaultCheatPath() {
  snes.config.path.cheat = "";
  syncUi();
}

void PathSettingsWindow::selectDataPath() {
  string path = utility.selectFolder("Default Export Data Path");
  if(path.length() > 0) {
    snes.config.path.data = path;
    syncUi();
  }
}

void PathSettingsWindow::defaultDataPath() {
  snes.config.path.data = "";
  syncUi();
}
