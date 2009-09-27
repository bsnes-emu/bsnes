#include "cheateditor.moc"
#include "statemanager.moc"

class ToolsWindow : public QbWindow {
  Q_OBJECT

public:
  QHBoxLayout *layout;
    QListWidget *list;
      QListWidgetItem *cheatEditor;
      QListWidgetItem *stateManager;
    QWidget *panel;
    QStackedLayout *panelLayout;

  void showCheatEditor();
  void showStateManager();
  ToolsWindow();

public slots:
  void listChanged();
} *toolsWindow;
