void DiskBrowser::ScreenshotPreview::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  if(screenshotName != "") {
    QImage image(screenshotName);
    painter.drawImage(0, 0, image);
  }
}

void DiskBrowser::onActivate(const QString &file, bool isDir) {
  if(browseMode == Folder) {
    setPath(file);
    return;
  }

  if(isDir) {
    QDir directory(file);
    directory.setNameFilters(QStringList() << "*.sfc" << "*.smc");
    QStringList list = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.count() == 1) {
      imageName = string() << file.toUtf8().constData() << "/" << list[0].toUtf8().constData();
      onLoad();
    } else {
      setPath(file);
    }
  } else {
    imageName = file.toUtf8().constData();
    onLoad();
  }
}

void DiskBrowser::onChange(const QString &file, bool isDir) {
  if(browseMode == Folder) {
    imageName = file.toUtf8().constData();
    load->setEnabled(isDir);
    applyPatch->setVisible(false);
    return;
  }

  imageName = queryAbsoluteImageName(file, isDir);
  if(imageName.length() == 0) {
    load->setEnabled(false);
    screenshot->screenshotName = "";
    info->setText("");
    applyPatch->setVisible(false);
  } else {
    load->setEnabled(true);
    screenshot->screenshotName = string() << utility.basepath(imageName) << "/" << utility.basename(imageName) << ".png";
    if(file::exists(screenshot->screenshotName) == false) screenshot->screenshotName = "";

    info->setText(utf8() << queryImageInformation(imageName));
    string patch = string() << utility.basepath(imageName) << utility.basename(imageName) << ".ups";
    applyPatch->setVisible(file::exists(patch));
  }
  screenshot->update();
}

void DiskBrowser::onLoad() {
  hide();
  switch(browseMode) { default:
    case Folder: activePath->selectPath(imageName); break;
    case AnyCartridge: utility.loadCartridge(imageName); break;
    case BaseCartridge: loaderWindow->selectBaseCartridge(imageName); break;
    case BsxCartridge: loaderWindow->selectSlot1Cartridge(imageName); break;
    case SufamiTurboCartridge1: loaderWindow->selectSlot1Cartridge(imageName); break;
    case SufamiTurboCartridge2: loaderWindow->selectSlot2Cartridge(imageName); break;
    case SuperGameBoyCartridge: loaderWindow->selectSlot1Cartridge(imageName); break;
  }
}

void DiskBrowser::chooseFolder(PathSettingWidget *widget, const char *title) {
  browseMode = Folder;
  activePath = widget;

  hide();
  panel()->hide();
  acceptButton()->setText("Choose");
  setWindowTitle(utf8() << title);
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters("Folders ()");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadAnyCartridge() {
  browseMode = AnyCartridge;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "All cartridges (*.sfc *.smc *.bs *.st" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadBaseCartridge() {
  browseMode = BaseCartridge;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load Base Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "SNES cartridges (*.sfc *.smc" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadBsxCartridge() {
  browseMode = BsxCartridge;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load BS-X Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "BS-X cartridges (*.bs" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadSufamiTurboCartridge1() {
  browseMode = SufamiTurboCartridge1;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load Slot-A Sufami Turbo Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Sufami Turbo cartridges (*.st" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadSufamiTurboCartridge2() {
  browseMode = SufamiTurboCartridge2;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load Slot-B Sufami Turbo Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Sufami Turbo Cartridges (*.st" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

void DiskBrowser::loadSuperGameBoyCartridge() {
  browseMode = SuperGameBoyCartridge;

  hide();
  panel()->show();
  acceptButton()->setText("Load");
  setWindowTitle("Load Super Game Boy Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Game Boy cartridges (*.gb *.gbc" << libsnesreader.filterList << ");;All files (*)");
  showAt(0.0, 0.0);
}

string DiskBrowser::queryAbsoluteImageName(const QString &file, bool isDir) {
  if(isDir) {
    QDir directory(file);
    directory.setNameFilters(QStringList() << "*.sfc" << "*.smc");
    QStringList list = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.count() == 1) {
      return string() << file.toUtf8().constData() << "/" << list[0].toUtf8().constData();
    } else {
      return "";
    }
  } else {
    return file.toUtf8().constData();
  }
}

string DiskBrowser::queryImageInformation(const char *filename) {
  string text;
  unsigned size = file::size(filename);
  text << "<b>" << utility.basename(imageName) << "</b>";
  text << "<small><table cellspacing='5'>";
  if(striend(filename, ".sfc") || striend(filename, ".smc")) {
    text << "<tr><td><b>ROM size:</b></td><td>" << size * 8 / 1024 / 1024 << "mbit</td></tr>";
  } else {
    text << "<tr><td><b>File size:</b></td><td>" << size * 8 / 1024 / 1024 << "mbit</td></tr>";
  }
  text << "</table></small>";
  return text;
}

DiskBrowser::DiskBrowser() {
  setMinimumSize(720, 480);

  layout = new QVBoxLayout;
  panel()->setLayout(layout);

  info = new QLabel;
  info->setFixedWidth(256);
  layout->addWidget(info);

  screenshot = new ScreenshotPreview;
  screenshot->setFixedSize(256, 239);
  layout->addWidget(screenshot);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  layout->addWidget(spacer);

  applyPatch = new QCheckBox("Apply UPS patch");
  applyPatch->setChecked(true);
  applyPatch->setVisible(false);
  applyPatch->setEnabled(false);
  layout->addWidget(applyPatch);

  connect(this, SIGNAL(itemActivated(const QString&, bool)), this, SLOT(onActivate(const QString&, bool)));
  connect(this, SIGNAL(itemChanged(const QString&, bool)), this, SLOT(onChange(const QString&, bool)));
  connect(load, SIGNAL(released()), this, SLOT(onLoad()));
}
