#include "video.moc"
#include "audio.moc"
#include "input.moc"
#include "paths.moc"
#include "advanced.moc"
#include "utility/inputcapture.moc"

class SettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
  QHBoxLayout *layout;
    QListWidget *list;
      QListWidgetItem *video;
      QListWidgetItem *audio;
      QListWidgetItem *input;
      QListWidgetItem *paths;
      QListWidgetItem *advanced;
    QWidget *panel;
    QStackedLayout *panelLayout;

  void setup();

public slots:
  void listChanged();
} *settingsWindow;
