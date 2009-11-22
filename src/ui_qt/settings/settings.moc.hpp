#include "video.moc"
#include "pixelshader.moc"
#include "audio.moc"
#include "input.moc"
#include "paths.moc"
#include "advanced.moc"

class SettingsWindow : public QbWindow {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QSplitter *splitter;
  QTreeWidget *list;
  QTreeWidgetItem *video;
  QTreeWidgetItem *audio;
  QTreeWidgetItem *input;
  QTreeWidgetItem *paths;
  QTreeWidgetItem *advanced;
  QWidget *panel;
  QStackedLayout *panelLayout;

  SettingsWindow();

public slots:
  void itemChanged();
} *settingsWindow;
