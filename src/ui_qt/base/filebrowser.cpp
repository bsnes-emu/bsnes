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
    if(striend(path, ".sfc") == false) return "";
    QDir directory(path);
    directory.setNameFilters(QStringList() << "*.sfc");
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

  if(file::exists(filename) && striend(filename, ".sfc")) {
    Cartridge::Information info;
    if(cartridge.information(filename, info)) {
      unsigned size = file::size(filename);
      string text;
      text << "<small><table>";
      text << "<tr><td><b>Title: </b></td><td>" << info.name << "</td></tr>";
      text << "<tr><td><b>Region: </b></td><td>" << info.region << "</td></tr>";
      text << "<tr><td><b>ROM: </b></td><td>" << info.romSize * 8 / 1024 / 1024 << "mbit</td></tr>";
      text << "<tr><td><b>RAM: </b></td><td>";
      info.ramSize ? text << info.ramSize * 8 / 1024 << "kbit</td></tr>" : text << "None</td></tr>";
      text << "</table></small>";
      previewInfo->setText(text);
    }

    string image = string() << nall::basename(filename) << ".png";
    previewImage->setStyleSheet(string() << "background: url(" << image << ") top left no-repeat;");

    string patch = string() << nall::basename(filename) << ".ups";
    previewApplyPatch->setVisible(file::exists(patch));
  } else {
    previewInfo->setText("");
    previewImage->setStyleSheet("background: transparent;");
    previewApplyPatch->hide();
  }
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
      if(strbegin(filter, "SNES cartridges")) {
        cartridge.loadNormal(filename);
      } else if(strbegin(filter, "BS-X cartridges")) {
        if(config().path.bsx == "") {
          loaderWindow->loadBsxCartridge("", filename);
        } else {
          cartridge.loadBsx(config().path.bsx, filename);
        }
      } else if(strbegin(filter, "Sufami Turbo cartridges")) {
        if(config().path.st == "") {
          loaderWindow->loadSufamiTurboCartridge("", filename, "");
        } else {
          cartridge.loadSufamiTurbo(config().path.st, filename, "");
        }
      } else if(strbegin(filter, "Game Boy cartridges")) {
        if(config().path.sgb == "") {
          loaderWindow->loadSuperGameBoyCartridge("", filename);
        } else {
          cartridge.loadSuperGameBoy(config().path.sgb, filename);
        }
      } else {
        cartridge.loadNormal(filename);
      }
    } else if(cartridgeMode == LoadBase) {
      loaderWindow->selectBaseCartridge(filename);
    } else if(cartridgeMode == LoadSlot1) {
      loaderWindow->selectSlot1Cartridge(filename);
    } else if(cartridgeMode == LoadSlot2) {
      loaderWindow->selectSlot2Cartridge(filename);
    }
  }
}
