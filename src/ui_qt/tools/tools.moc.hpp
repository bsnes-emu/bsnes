#include "cheateditor.moc"
#include "statemanager.moc"

class ToolsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
  QHBoxLayout *layout;
    QListWidget *list;
      QListWidgetItem *cheatEditor;
      QListWidgetItem *stateManager;
    QWidget *panel;
    QStackedLayout *panelLayout;

  void setup();
  void showCheatEditor();
  void showStateManager();

public slots:
  void listChanged();
} *toolsWindow;
