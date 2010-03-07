#include "filebrowser.moc"
FileBrowser *fileBrowser;

void FileBrowser::chooseFolder() {
  previewFrame->hide();
  showFolder();
}

void FileBrowser::loadCartridge(CartridgeMode mode, signed filterIndex) {
  cartridgeMode = mode;
  onChange = bind(&FileBrowser::onChangeCartridge, this);
  onActivate = bind(&FileBrowser::onAcceptCartridge, this);
  onAccept = bind(&FileBrowser::onAcceptCartridge, this);

  setPath(config().path.rom == "" ? config().path.current.cartridge : config().path.rom);
  setNameFilters(string()
  << "SNES cartridges (*.sfc" << reader.extensionList << reader.compressionList << ")\n"
  << "BS-X cartridges (*.bs" << reader.compressionList << ")\n"
  << "Sufami Turbo cartridges (*.st" << reader.compressionList << ")\n"
  << "Game Boy cartridges (*.gb *.sgb *.gbc" << reader.compressionList << ")\n"
  << "All files (*)"
  );
  previewFrame->show();
  filterBox->setCurrentIndex(filterIndex == -1 ? config().path.current.filter : filterIndex);
  showLoad();
}

void FileBrowser::change(const string &path) {
  if(onChange) onChange(path);
}

void FileBrowser::activate(const string &path) {
  if(onActivate) onActivate(path);
}

void FileBrowser::accept(const string &path) {
  if(onAccept) onAccept(path);
}

void FileBrowser::toggleApplyPatch() {
  config().file.applyPatches = previewApplyPatch->isChecked();
}

FileBrowser::FileBrowser() {
  setObjectName("file-browser");
  resize(800, 480);
  setGeometryString(&config().geometry.fileBrowser);
  application.windowList.add(this);

  previewLayout = new QVBoxLayout;
  previewLayout->setAlignment(Qt::AlignTop);
  previewFrame->setLayout(previewLayout);

  previewInfo = new QLabel;
  previewLayout->addWidget(previewInfo);

  previewImage = new QWidget;
  previewImage->setFixedSize(256, 239);
  previewLayout->addWidget(previewImage);

  previewSpacer = new QWidget;
  previewSpacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  previewLayout->addWidget(previewSpacer);

  previewApplyPatch = new QCheckBox("Apply Patch");
  previewApplyPatch->setVisible(false);
  previewApplyPatch->setChecked(config().file.applyPatches);
  previewLayout->addWidget(previewApplyPatch);

  connect(this, SIGNAL(changed(const string&)), this, SLOT(change(const string&)));
  connect(this, SIGNAL(activated(const string&)), this, SLOT(activate(const string&)));
  connect(this, SIGNAL(accepted(const string&)), this, SLOT(accept(const string&)));
  connect(previewApplyPatch, SIGNAL(stateChanged(int)), this, SLOT(toggleApplyPatch()));
}

//

string FileBrowser::resolveFilename(const string &path) {
  if(QDir(path).exists()) {
    string filter;
    if(striend(path, ".sfc")) filter = "*.sfc";
    if(striend(path, ".bs" )) filter = "*.bs";
    if(striend(path, ".st" )) filter = "*.st";
    if(striend(path, ".gb" )) filter = "*.gb";
    if(striend(path, ".sgb")) filter = "*.sgb";
    if(striend(path, ".gbc")) filter = "*.gbc";
    if(filter == "") return "";

    QDir directory(path);
    directory.setNameFilters(QStringList() << filter);
    QStringList list = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.count() == 1) return string() << path << "/" << list[0].toUtf8().constData();
    return "";
  } else {
    return path;
  }
}

void FileBrowser::onChangeCartridge(const string &path) {
  string filename;
  if(QDir(path).exists()) filename = resolveFilename(path);
  else filename = path;

  string info;
  string image = string() << nall::basename(filename) << ".png";
  string patch = string() << nall::basename(filename) << ".ups";

  if(file::exists(filename)) {
    if(striend(filename, ".sfc")) {
      Cartridge::Information cartinfo;
      if(cartridge.information(filename, cartinfo)) {
        info << "<small><table>";
        info << "<tr><td><b>Title: </b></td><td>" << cartinfo.name << "</td></tr>";
        info << "<tr><td><b>Region: </b></td><td>" << cartinfo.region << "</td></tr>";
        info << "<tr><td><b>ROM: </b></td><td>" << cartinfo.romSize * 8 / 1024 / 1024 << "mbit</td></tr>";
        info << "<tr><td><b>RAM: </b></td><td>";
        cartinfo.ramSize ? info << cartinfo.ramSize * 8 / 1024 << "kbit</td></tr>" : info << "None</td></tr>";
        info << "</table></small>";
      }
    } else if(striend(filename, ".st")) {
      unsigned size = file::size(filename);
      info << "<small><table>";
      info << "<tr><td><b>ROM: </b></td><td>" << size * 8 / 1024 / 1024 << "mbit</td></tr>";
      info << "</table></small>";
    }
  }

  if(info == "") info = "<small><font color='#808080'>No preview available</font></small>";
  previewInfo->setText(info);
  previewImage->setStyleSheet(string() << "background: url(" << image << ") center left no-repeat;");
  previewApplyPatch->setVisible(file::exists(patch));
}

void FileBrowser::onAcceptCartridge(const string &path) {
  string filename;
  if(QDir(path).exists()) {
    filename = resolveFilename(path);
  } else {
    filename = path;
  }

  if(file::exists(filename)) {
    close();
    config().path.current.cartridge = fileSystemModel->rootPath().toUtf8().constData();

    if(cartridgeMode == LoadDirect) {
      config().path.current.filter = filterBox->currentIndex();
      string filter = filterBox->currentText().toUtf8().constData();

      if(0);
      //file extension detection
      else if(striend(filename, ".sfc")) acceptNormal(filename);
      else if(striend(filename, ".bs"))  acceptBsx(filename);
      else if(striend(filename, ".st"))  acceptSufamiTurbo(filename);
      else if(striend(filename, ".gb"))  acceptSuperGameBoy(filename);
      else if(striend(filename, ".sgb")) acceptSuperGameBoy(filename);
      else if(striend(filename, ".gbc")) acceptSuperGameBoy(filename);
      //filter detection
      else if(strbegin(filter, "SNES cartridges")) acceptNormal(filename);
      else if(strbegin(filter, "BS-X cartridges")) acceptBsx(filename);
      else if(strbegin(filter, "Sufami Turbo cartridges")) acceptSufamiTurbo(filename);
      else if(strbegin(filter, "Game Boy cartridges")) acceptSuperGameBoy(filename);
      //fallback behavior
      else acceptNormal(filename);
    } else if(cartridgeMode == LoadBase) {
      loaderWindow->selectBaseCartridge(filename);
    } else if(cartridgeMode == LoadSlot1) {
      loaderWindow->selectSlot1Cartridge(filename);
    } else if(cartridgeMode == LoadSlot2) {
      loaderWindow->selectSlot2Cartridge(filename);
    }
  }
}

void FileBrowser::acceptNormal(const string &filename) {
  cartridge.loadNormal(filename);
}

void FileBrowser::acceptBsx(const string &filename) {
  if(config().path.bsx == "") {
    loaderWindow->loadBsxCartridge("", filename);
  } else {
    cartridge.loadBsx(config().path.bsx, filename);
  }
}

void FileBrowser::acceptSufamiTurbo(const string &filename) {
  if(config().path.st == "") {
    loaderWindow->loadSufamiTurboCartridge("", filename, "");
  } else {
    cartridge.loadSufamiTurbo(config().path.st, filename, "");
  }
}

void FileBrowser::acceptSuperGameBoy(const string &filename) {
  if(config().path.sgb == "") {
    loaderWindow->loadSuperGameBoyCartridge("", filename);
  } else {
    cartridge.loadSuperGameBoy(config().path.sgb, filename);
  }
}
