#ifndef NALL_QT_FILEDIALOG_HPP
#define NALL_QT_FILEDIALOG_HPP

#include <nall/string.hpp>
#include <nall/qt/window.moc.hpp>

namespace nall {

class FileView : public QListView {
  Q_OBJECT

protected:
  void keyPressEvent(QKeyEvent*);

signals:
  void changed(const QModelIndex&);
  void browseUp();

protected slots:
  void currentChanged(const QModelIndex&, const QModelIndex&);
};

class FileDialog : public Window {
  Q_OBJECT

public:
  void showLoad();
  void showSave();
  void showFolder();

  void setPath(string path);
  void setNameFilters(const string &filters);
  FileDialog();

signals:
  void changed(const string&);
  void activated(const string&);
  void accepted(const string&);
  void rejected();

protected slots:
  void fileViewChange(const QModelIndex&);
  void fileViewActivate(const QModelIndex&);
  void pathBoxChanged();
  void filterBoxChanged();
  void browseUp();
  void acceptAction();
  void rejectAction();

protected:
  QVBoxLayout *layout;
  QHBoxLayout *navigationLayout;
  QComboBox *pathBox;
  QPushButton *newFolderButton;
  QPushButton *upFolderButton;
  QHBoxLayout *browseLayout;
  QFileSystemModel *fileSystemModel;
  FileView *fileView;
  QGroupBox *previewFrame;
  QLineEdit *fileNameEdit;
  QHBoxLayout *controlLayout;
  QComboBox *filterBox;
  QPushButton *optionsButton;
  QPushButton *acceptButton;
  QPushButton *rejectButton;

  bool lock;
};

inline void FileView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  QAbstractItemView::currentChanged(current, previous);
  emit changed(current);
}

inline void FileView::keyPressEvent(QKeyEvent *event) {
  //enhance consistency: force OS X to act like Windows and Linux; enter = activate item
  if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    emit activated(currentIndex());
    return;
  }

  //simulate popular file manager behavior; backspace = go up one directory
  if(event->key() == Qt::Key_Backspace) {
    emit browseUp();
    return;
  }

  //fallback: unrecognized keypresses get handled by the widget itself
  QListView::keyPressEvent(event);
}

inline void FileDialog::showLoad() {
  acceptButton->setText("Load");
  fileNameEdit->hide();
  filterBox->show();
  show();
}

inline void FileDialog::showSave() {
  acceptButton->setText("Save");
  fileNameEdit->show();
  filterBox->show();
  show();
}

inline void FileDialog::showFolder() {
  acceptButton->setText("Choose");
  fileNameEdit->hide();
  filterBox->hide();
  setNameFilters("Folders ()");
  show();
}

inline void FileDialog::fileViewChange(const QModelIndex &index) {
  string path = fileSystemModel->filePath(index).toUtf8().constData();
  if(path == fileSystemModel->rootPath().toUtf8().constData()) path = "";
  fileNameEdit->setText(notdir(path));
  emit changed(path);
}

inline void FileDialog::fileViewActivate(const QModelIndex &index) {
  string path = fileSystemModel->filePath(index).toUtf8().constData();
  if(fileSystemModel->isDir(index)) {
    emit activated(path);
    setPath(path);
  } else {
    emit activated(path);
    close();
  }
}

inline void FileDialog::pathBoxChanged() {
  if(lock) return;
  setPath(pathBox->currentText().toUtf8().constData());
}

inline void FileDialog::filterBoxChanged() {
  if(lock) return;
  string filters = filterBox->currentText().toUtf8().constData();
  if(filters.length() == 0) {
    fileSystemModel->setNameFilters(QStringList() << "*");
  } else {
    filters = substr(filters, strpos(filters, "("));
    ltrim(filters, "(");
    rtrim(filters, ")");
    lstring part;
    part.split(" ", filters);
    QStringList list;
    for(unsigned i = 0; i < part.size(); i++) list << part[i];
    fileSystemModel->setNameFilters(list);
  }
}

inline void FileDialog::browseUp() {
  if(pathBox->count() > 1) pathBox->setCurrentIndex(1);
}

inline void FileDialog::setPath(string path) {
  lock = true;

  if(QDir(path).exists()) {
    newFolderButton->setEnabled(true);
  } else {
    newFolderButton->setEnabled(false);
    path = "";
  }

  fileSystemModel->setRootPath(path);
  fileView->setRootIndex(fileSystemModel->index(path));
  fileView->setCurrentIndex(fileView->rootIndex());
  fileView->setFocus();

  pathBox->clear();
  if(path.length() > 0) {
    QDir directory(path);
    while(true) {
      pathBox->addItem(directory.absolutePath());
      if(directory.isRoot()) break;
      directory.cdUp();
    }
  }
  pathBox->addItem("<root>");
  fileNameEdit->setText("");

  lock = false;
}

inline void FileDialog::setNameFilters(const string &filters) {
  lock = true;

  lstring list;
  list.split("\n", filters);

  filterBox->clear();
  for(unsigned i = 0; i < list.size(); i++) {
    filterBox->addItem(list[i]);
  }

  lock = false;
  filterBoxChanged();
}

inline void FileDialog::acceptAction() {
  string path = fileSystemModel->rootPath().toUtf8().constData();
  path << "/" << notdir(fileNameEdit->text().toUtf8().constData());
  rtrim(path, "/");
  if(QDir(path).exists()) {
    emit accepted(path);
    setPath(path);
  } else {
    emit accepted(path);
    close();
  }
}

inline void FileDialog::rejectAction() {
  emit rejected();
  close();
}

inline FileDialog::FileDialog() {
  resize(640, 360);

  layout = new QVBoxLayout;
  setLayout(layout);

  navigationLayout = new QHBoxLayout;
  layout->addLayout(navigationLayout);

  pathBox = new QComboBox;
  pathBox->setEditable(true);
  pathBox->setMinimumContentsLength(16);
  pathBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
  pathBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  navigationLayout->addWidget(pathBox);

  newFolderButton = new QPushButton;
  newFolderButton->setIconSize(QSize(16, 16));
  newFolderButton->setIcon(QIcon(":/16x16/folder-new.png"));
  navigationLayout->addWidget(newFolderButton);

  upFolderButton = new QPushButton;
  upFolderButton->setIconSize(QSize(16, 16));
  upFolderButton->setIcon(QIcon(":/16x16/go-up.png"));
  navigationLayout->addWidget(upFolderButton);

  browseLayout = new QHBoxLayout;
  layout->addLayout(browseLayout);

  fileSystemModel = new QFileSystemModel;
  fileSystemModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
  fileSystemModel->setNameFilterDisables(false);

  fileView = new FileView;
  fileView->setModel(fileSystemModel);
  fileView->setIconSize(QSize(16, 16));
  browseLayout->addWidget(fileView);

  previewFrame = new QGroupBox;
  previewFrame->hide();
  browseLayout->addWidget(previewFrame);

  fileNameEdit = new QLineEdit;
  layout->addWidget(fileNameEdit);

  controlLayout = new QHBoxLayout;
  controlLayout->setAlignment(Qt::AlignRight);
  layout->addLayout(controlLayout);

  filterBox = new QComboBox;
  filterBox->setMinimumContentsLength(16);
  filterBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
  filterBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  controlLayout->addWidget(filterBox);

  optionsButton = new QPushButton("Options");
  optionsButton->hide();
  controlLayout->addWidget(optionsButton);

  acceptButton = new QPushButton("Ok");
  controlLayout->addWidget(acceptButton);

  rejectButton = new QPushButton("Cancel");
  controlLayout->addWidget(rejectButton);

  lock = false;
  connect(pathBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pathBoxChanged()));
  connect(upFolderButton, SIGNAL(released()), this, SLOT(browseUp()));
  connect(fileView, SIGNAL(changed(const QModelIndex&)), this, SLOT(fileViewChange(const QModelIndex&)));
  connect(fileView, SIGNAL(activated(const QModelIndex&)), this, SLOT(fileViewActivate(const QModelIndex&)));
  connect(fileView, SIGNAL(browseUp()), this, SLOT(browseUp()));
  connect(filterBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterBoxChanged()));
  connect(acceptButton, SIGNAL(released()), this, SLOT(acceptAction()));
  connect(rejectButton, SIGNAL(released()), this, SLOT(rejectAction()));
}

}

#endif
