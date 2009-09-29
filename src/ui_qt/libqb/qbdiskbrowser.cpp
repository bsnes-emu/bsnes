void QbDiskBrowserView::keyPressEvent(QKeyEvent *event) {
  //enhance consistency: OS X by default doesn't activate items for these keypresses
  if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    parent.activateItem(currentIndex());
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

void QbDiskBrowserView::keyReleaseEvent(QKeyEvent *event) {
  //act like a modal dialog; close window on escape keypress
  if(event->key() == Qt::Key_Escape) {
    parent.close();
    return;
  }

  //fallback: unrecognized keypresses get handled by the widget itself
  QTreeView::keyReleaseEvent(event);
}

void QbDiskBrowserView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  QAbstractItemView::currentChanged(current, previous);
  emit changed(current);
}

QbDiskBrowserView::QbDiskBrowserView(QbDiskBrowser &parent_) : parent(parent_) {
}

QGroupBox *QbDiskBrowser::panel() { return group; }
QPushButton *QbDiskBrowser::acceptButton() { return load; }

void QbDiskBrowser::setPath(const QString &reqPath) {
  disconnect(path, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePath()));

  QString effectivePath = reqPath;
  if(effectivePath == "<root>") effectivePath = "";
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

void QbDiskBrowser::setNameFilters(const QString &filters) {
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

void QbDiskBrowser::cdUp() {
  //if we aren't already at the root node, select the second node, which is one path higher than the current
  if(path->count() > 1) path->setCurrentIndex(1);
}

void QbDiskBrowser::updatePath() {
  setPath(path->currentText());
}

void QbDiskBrowser::updateFilter() {
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

void QbDiskBrowser::activateItem(const QModelIndex &item) {
  emit itemActivated(model->filePath(item), model->isDir(item));
}

void QbDiskBrowser::changeItem(const QModelIndex &item) {
  emit itemChanged(model->filePath(item), model->isDir(item));
}

QbDiskBrowser::QbDiskBrowser() {
  layout = new QVBoxLayout;
  setLayout(layout);

  topLayout = new QHBoxLayout;
  layout->addLayout(topLayout);

  browseLayout = new QVBoxLayout;
  topLayout->addLayout(browseLayout);

  path = new QComboBox;
  browseLayout->addWidget(path);

  view = new QbDiskBrowserView(*this);
  view->setIconSize(QSize(16, 16));
  browseLayout->addWidget(view);

  panelLayout = new QVBoxLayout;
  topLayout->addLayout(panelLayout);

  group = new QGroupBox;
  group->hide();
  panelLayout->addWidget(group);

  controlLayout = new QHBoxLayout;
  layout->addLayout(controlLayout);

  filter = new QComboBox;
  filter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  controlLayout->addWidget(filter);

  options = new QPushButton("Options");
  options->setEnabled(false);
  controlLayout->addWidget(options);

  QMenu *menu = new QMenu;
  options->setMenu(menu);

  load = new QPushButton("Load");
  load->setEnabled(false);
  controlLayout->addWidget(load);

  cancel = new QPushButton("Cancel");
  controlLayout->addWidget(cancel);

  model = new QFileSystemModel;
  model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
  model->setNameFilterDisables(false);

  view->setModel(model);
  view->setAllColumnsShowFocus(true);
  view->setUniformRowHeights(true);
  view->setSortingEnabled(true);
  view->sortByColumn(0, Qt::AscendingOrder);  //sort files by name
  view->setColumnHidden(1, true);             //hide size
  view->setColumnHidden(2, true);             //hide type
  view->setColumnHidden(3, true);             //hide last modified date
  view->setHeaderHidden(true);

  connect(view, SIGNAL(cdUp()), this, SLOT(cdUp()));
  connect(view, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateItem(const QModelIndex&)));
  connect(view, SIGNAL(changed(const QModelIndex&)), this, SLOT(changeItem(const QModelIndex&)));
  connect(cancel, SIGNAL(released()), this, SLOT(hide()));
}
