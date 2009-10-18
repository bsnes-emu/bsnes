//=============
//FolderCreator
//=============

FolderCreator::FolderCreator() {
  window = new QbWindow(config.geometry.folderCreator);
  window->setObjectName("folder-creator");
  window->setWindowTitle("Create New Folder");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  layout->setAlignment(Qt::AlignTop);
  window->setLayout(layout);

  label = new QLabel("Folder name:");
  layout->addWidget(label);

  name = new QLineEdit;
  layout->addWidget(name);

  controlLayout = new QHBoxLayout;
  controlLayout->setAlignment(Qt::AlignRight);
  layout->addLayout(controlLayout);

  ok = new QPushButton("Ok");
  controlLayout->addWidget(ok);

  cancel = new QPushButton("Cancel");
  controlLayout->addWidget(cancel);

  connect(name, SIGNAL(returnPressed()), this, SLOT(createFolder()));
  connect(ok, SIGNAL(released()), this, SLOT(createFolder()));
  connect(cancel, SIGNAL(released()), window, SLOT(hide()));
}

void FolderCreator::show() {
  name->setText("");
  window->show();
  name->setFocus();
}

void FolderCreator::createFolder() {
  if(name->text().length() == 0) {
    QMessageBox::warning(0, "Create New Folder", "<b>Note:</b> you must enter a folder name.");
  } else {
    string folderName = string()
    << diskBrowser->path->currentText().toUtf8().constData()
    << "/"
    << name->text().toUtf8().constData();

    if(mkdir(folderName) == 0) {
      window->hide();
    } else {
      QMessageBox::warning(0, "Create new Folder", "<b>Error:</b> failed to create folder. Please ensure only valid characters were used in the folder name.");
    }
  }
}

//===============
//DiskBrowserView
//===============

void DiskBrowserView::keyPressEvent(QKeyEvent *event) {
  //enhance consistency: OS X by default doesn't activate items for these keypresses
  if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    emit activated(currentIndex());
    return;
  }

  //simulate popular file managers; backspace = go up one directory
  if(event->key() == Qt::Key_Backspace) {
    emit cdUp();
    return;
  }

  //fallback: unrecognized keypresses get handled by the widget itself
  QTreeView::keyPressEvent(event);
}

void DiskBrowserView::keyReleaseEvent(QKeyEvent *event) {
  //act like a modal dialog; close window on escape keypress
  if(event->key() == Qt::Key_Escape) {
    emit escape();
    return;
  }

  //fallback: unrecognized keypresses get handled by the widget itself
  QTreeView::keyReleaseEvent(event);
}

void DiskBrowserView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  QAbstractItemView::currentChanged(current, previous);
  emit changed(current);
}

//================
//DiskBrowserImage
//================

void DiskBrowserImage::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  if(name != "") {
    QImage image(name);
    painter.drawImage(0, 0, image);
  }
}

//===========
//DiskBrowser
//===========

void DiskBrowser::chooseFolder(PathSettingWidget *widget, const char *title) {
  browseMode = Folder;
  activePath = widget;
  window->hide();
  group->hide();
  ok->setText("Choose");
  window->setWindowTitle(utf8() << title);
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters("Folders ()");
  window->show();
}

void DiskBrowser::loadCartridge() {
  browseMode = Cartridge;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "SNES cartridges (*.sfc *.smc" << reader.filterList << ");;All files (*)");
  window->show();
}

void DiskBrowser::loadBaseCartridge() {
  browseMode = BaseCartridge;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load Base Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "SNES cartridges (*.sfc *.smc" << reader.filterList << ");;All files (*)");
  window->show();
}

void DiskBrowser::loadBsxCartridge() {
  browseMode = BsxCartridge;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load BS-X Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "BS-X cartridges (*.bs" << reader.filterList << ");;All files (*)");
  window->show();
}

void DiskBrowser::loadSufamiTurboCartridge1() {
  browseMode = SufamiTurboCartridge1;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load Slot-A Sufami Turbo Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Sufami Turbo cartridges (*.st" << reader.filterList << ");;All files (*)");
  window->show();
}

void DiskBrowser::loadSufamiTurboCartridge2() {
  browseMode = SufamiTurboCartridge2;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load Slot-B Sufami Turbo Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Sufami Turbo Cartridges (*.st" << reader.filterList << ");;All files (*)");
  window->show();
}

void DiskBrowser::loadSuperGameBoyCartridge() {
  browseMode = SuperGameBoyCartridge;
  window->hide();
  group->show();
  ok->setText("Load");
  window->setWindowTitle("Load Super Game Boy Cartridge");
  setPath(utf8() << (config.path.rom != "" ? config.path.rom : config.path.current));
  setNameFilters(utf8() << "Game Boy cartridges (*.gb *.gbc" << reader.filterList << ");;All files (*)");
  window->show();
}

string DiskBrowser::queryImageInformation() {
  string text;
  string filename;
  if(currentFilename(filename) == true) {
    if(striend(filename, ".sfc") || striend(filename, ".smc")) {
      unsigned size = file::size(filename);
      text << "<b>" << notdir(nall::basename(filename)) << "</b>";
      text << "<small><table cellspacing='5'>";
      text << "<tr><td><b>ROM size:</b></td><td>" << size * 8 / 1024 / 1024 << "mbit</td></tr>";
      text << "</table></small>";
    }
  }
  return text;
}

void DiskBrowser::activateItem(const QModelIndex &item) {
  if(browseMode == Folder) {
    setPath(model->filePath(item));
  } else {
    loadSelected();
  }
}

void DiskBrowser::changeItem(const QModelIndex &item) {
  if(browseMode == Folder) {
    ok->setEnabled(model->isDir(item));
    image->name = "";
    info->setText("");
    applyPatch->setVisible(false);
  } else {
    string filename;
    currentFilename(filename);

    if(filename.length() == 0) {
      //nothing selected?
      ok->setEnabled(false);
      image->name = "";
      info->setText("");
      applyPatch->setVisible(false);
    } else {
      ok->setEnabled(true);
      image->name = nall::basename(filename) << ".png";
      if(file::exists(image->name) == false) image->name = "";
      info->setText(utf8() << queryImageInformation());
      string patch = nall::basename(filename) << ".ups";
      applyPatch->setVisible(file::exists(patch));
    }
  }

  image->update();
}

void DiskBrowser::loadSelected() {
  string filename;
  bool loadable = currentFilename(filename);

  if(browseMode == Folder || loadable == true) {
    QModelIndex item = view->currentIndex();
    config.path.current = dir(model->filePath(item).toUtf8().constData());
    window->hide();

    switch(browseMode) { default:
      case Folder: activePath->selectPath(filename); break;
      case Cartridge: utility.loadCartridgeNormal(filename); break;
      case BaseCartridge: loaderWindow->selectBaseCartridge(filename); break;
      case BsxCartridge: loaderWindow->selectSlot1Cartridge(filename); break;
      case SufamiTurboCartridge1: loaderWindow->selectSlot1Cartridge(filename); break;
      case SufamiTurboCartridge2: loaderWindow->selectSlot2Cartridge(filename); break;
      case SuperGameBoyCartridge: loaderWindow->selectSlot1Cartridge(filename); break;
    }
  } else {
    //this is a standard folder in ROM loading mode; enter into the folder
    QModelIndex item = view->currentIndex();
    setPath(model->filePath(item));
  }
}

//

void DiskBrowser::setPath(const QString &reqPath) {
  disconnect(path, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePath()));

  QString effectivePath = reqPath;
  if(effectivePath == "<root>") {
    effectivePath = "";
    newFolder->setEnabled(false);
  } else {
    newFolder->setEnabled(true);
  }
  path->clear();
  model->setRootPath(effectivePath);
  view->setRootIndex(model->index(effectivePath));
  view->setCurrentIndex(view->rootIndex());
  view->setFocus();

  if(effectivePath.length() > 0) {
    QDir directory(effectivePath);
    while(true) {
      path->addItem(directory.absolutePath());
      if(directory.isRoot()) break;
      directory.cdUp();
    }
  }
  path->addItem("<root>");

  connect(path, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePath()));
}

void DiskBrowser::setNameFilters(const QString &filters) {
  disconnect(filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));

  filter->clear();

  string filterData = filters.toUtf8().constData();
  lstring filterPart;
  filterPart.split(";;", filterData);

  for(unsigned i = 0; i < filterPart.size(); i++) {
    filter->addItem(utf8() << filterPart[i]);
  }

  connect(filter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
  updateFilter();
}

void DiskBrowser::cdUp() {
  folderCreator->window->hide();
  //if we aren't already at the root node, select the second node, which is one path higher than the current
  if(path->count() > 1) path->setCurrentIndex(1);
}

void DiskBrowser::updatePath() {
  setPath(path->currentText());
}

void DiskBrowser::updateFilter() {
  QString currentText = filter->currentText();
  if(currentText.length() == 0) {
    model->setNameFilters(QStringList() << "*");  
  } else {
    string filters = currentText.toUtf8().constData();
    filters = substr(filters, strpos(filters, "("));
    ltrim(filters, "(");
    rtrim(filters, ")");
    lstring filterPart;
    filterPart.split(" ", filters);
    QStringList filterList;
    for(unsigned i = 0; i < filterPart.size(); i++) filterList << (const char*)filterPart[i];
    model->setNameFilters(filterList);
  }
}

//true means filename can be loaded directly
//false means it cannot (eg this is a folder and we are attempting to load a ROM)
bool DiskBrowser::currentFilename(string &filename) {
  bool loadable = false;
  QModelIndex item = view->currentIndex();
  filename = model->filePath(item).toUtf8().constData();

  if(browseMode != Folder) {
    if(model->isDir(item) == true) {
      QDir directory(utf8() << filename);
      directory.setNameFilters(QStringList() << "*.sfc" << "*.smc");
      QStringList list = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
      if(list.count() == 1) {
        filename << "/" << list[0].toUtf8().constData();
        loadable = true;
      }
    } else {
      loadable = true;
    }
  }

  return loadable;
}

DiskBrowser::DiskBrowser() {
  window = new QbWindow(config.geometry.diskBrowser);
  window->setObjectName("disk-browser");
  window->resize(720, 480);

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  window->setLayout(layout);

  topLayout = new QHBoxLayout;
  layout->addLayout(topLayout);

  browseLayout = new QVBoxLayout;
  topLayout->addLayout(browseLayout);

  pathLayout = new QHBoxLayout;
  browseLayout->addLayout(pathLayout);

  path = new QComboBox;
  path->setMinimumContentsLength(16);
  path->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
  path->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  pathLayout->addWidget(path);

  newFolder = new QPushButton;
  newFolder->setIconSize(QSize(16, 16));
  newFolder->setIcon(QIcon(":/16x16/folder-new.png"));
  pathLayout->addWidget(newFolder);

  view = new DiskBrowserView;
  view->setIconSize(QSize(16, 16));
  browseLayout->addWidget(view);

  panelLayout = new QVBoxLayout;
  topLayout->addLayout(panelLayout);

  group = new QGroupBox;
  panelLayout->addWidget(group);

  groupLayout = new QVBoxLayout;
  group->setLayout(groupLayout);

  info = new QLabel;
  info->setFixedWidth(256);
  groupLayout->addWidget(info);

  image = new DiskBrowserImage;
  image->setFixedSize(256, 239);
  groupLayout->addWidget(image);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  groupLayout->addWidget(spacer);

  applyPatch = new QCheckBox("Apply UPS patch");
  applyPatch->setChecked(true);
  applyPatch->setEnabled(false);
  groupLayout->addWidget(applyPatch);

  controlLayout = new QHBoxLayout;
  layout->addLayout(controlLayout);

  filter = new QComboBox;
  filter->setMinimumContentsLength(16);
  filter->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
  filter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  controlLayout->addWidget(filter);

  options = new QPushButton("Options");
  options->setEnabled(false);
  controlLayout->addWidget(options);

  QMenu *menu = new QMenu;
  options->setMenu(menu);

  ok = new QPushButton("Ok");
  ok->setEnabled(false);
  controlLayout->addWidget(ok);

  cancel = new QPushButton("Cancel");
  controlLayout->addWidget(cancel);

  model = new QFileSystemModel;
  model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
  model->setNameFilterDisables(false);

  view->setModel(model);
  view->setExpandsOnDoubleClick(false);
  view->setAllColumnsShowFocus(true);
  view->setUniformRowHeights(true);
  view->setSortingEnabled(true);
  view->sortByColumn(0, Qt::AscendingOrder);
  view->setColumnHidden(1, true);
  view->setColumnHidden(2, true);
  view->setColumnHidden(3, true);
  view->setHeaderHidden(true);

  folderCreator = new FolderCreator;

  connect(newFolder, SIGNAL(released()), folderCreator, SLOT(show()));
  connect(view, SIGNAL(cdUp()), this, SLOT(cdUp()));
  connect(view, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateItem(const QModelIndex&)));
  connect(view, SIGNAL(changed(const QModelIndex&)), this, SLOT(changeItem(const QModelIndex&)));
  connect(view, SIGNAL(escape()), window, SLOT(hide()));
  connect(ok, SIGNAL(released()), this, SLOT(loadSelected()));
  connect(cancel, SIGNAL(released()), window, SLOT(hide()));
}
