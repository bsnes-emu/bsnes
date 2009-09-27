#include "video.moc"
#include "audio.moc"
#include "input.moc"
#include "paths.moc"
#include "advanced.moc"
#include "utility/inputcapture.moc"

class SettingsWindow : public QbWindow {
  Q_OBJECT

public:
  QHBoxLayout *layout;
    QListWidget *list;
      QListWidgetItem *video;
      QListWidgetItem *audio;
      QListWidgetItem *input;
      QListWidgetItem *paths;
      QListWidgetItem *advanced;
    QWidget *panel;
    QStackedLayout *panelLayout;

  SettingsWindow();

public slots:
  void listChanged();
} *settingsWindow;
