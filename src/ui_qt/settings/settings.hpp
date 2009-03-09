#include "video.moc"
#include "audio.moc"
#include "input.moc"
#include "paths.moc"
#include "cheateditor.moc"
#include "advanced.moc"

#include "utility/inputcapture.moc"
#include "utility/codeeditor.moc"

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
        QListWidgetItem *cheatcodes;
        QListWidgetItem *advanced;
      QWidget *panel;
        QStackedLayout *panelLayout;

  void setup();
  void show();

public slots:
  void listChanged();
} *winSettings;
