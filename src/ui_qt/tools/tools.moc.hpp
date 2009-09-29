#include "cheateditor.moc"
#include "statemanager.moc"

class ToolsWindow : public QbWindow {
  Q_OBJECT

public:
  QHBoxLayout *layout;
    QTreeWidget *list;
      QTreeWidgetItem *cheatEditor;
      QTreeWidgetItem *stateManager;
    QWidget *panel;
    QStackedLayout *panelLayout;

  void showCheatEditor();
  void showStateManager();
  ToolsWindow();

public slots:
  void itemChanged();
} *toolsWindow;
