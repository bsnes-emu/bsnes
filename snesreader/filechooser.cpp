#include "filechooser.moc.hpp"
#include "filechooser.moc"

//FileChooser is implemented as a modal QWidget instead of a QDialog
//due to a bug in Qt 4.6.0 (QTBUG-7188); which causes the FileChooser
//to not refresh when a QTimer is active from the main application.
string FileChooser::exec() {
  if(list.size() == 0) return "";
  if(list.size() == 1) return list[0];

  listWidget->clear();
  for(unsigned i = 0; i < list.size(); i++) {
    listWidget->addItem(list[i]);
  }
  listWidget->sortItems(Qt::AscendingOrder);
  listWidget->setCurrentRow(0);
  listWidget->setFocus();

  name = "";
  setWindowModality(Qt::ApplicationModal);
  show();
  while(isVisible()) QApplication::processEvents();
  setWindowModality(Qt::NonModal);
  return name;
}

void FileChooser::load() {
  QListWidgetItem *item = listWidget->currentItem();
  if(item) name = item->text().toUtf8().constData();
  close();
}

FileChooser::FileChooser() {
  setWindowTitle("Select Cartridge To Load");
  setMinimumWidth(480);
  setMinimumHeight(320);

  layout = new QVBoxLayout;
  setLayout(layout);

  listWidget = new QListWidget;
  layout->addWidget(listWidget);

  controlLayout = new QHBoxLayout;
  controlLayout->setAlignment(Qt::AlignRight);
  layout->addLayout(controlLayout);

  okButton = new QPushButton("Ok");
  controlLayout->addWidget(okButton);

  cancelButton = new QPushButton("Cancel");
  controlLayout->addWidget(cancelButton);

  connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(load()));
  connect(okButton, SIGNAL(released()), this, SLOT(load()));
  connect(cancelButton, SIGNAL(released()), this, SLOT(close()));
}
