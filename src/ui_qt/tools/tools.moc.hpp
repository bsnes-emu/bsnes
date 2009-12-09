class ToolsWindow : public QbWindow {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QSplitter *splitter;
  QTreeWidget *list;
  QTreeWidgetItem *cheatEditor;
  QTreeWidgetItem *cheatFinder;
  QTreeWidgetItem *stateManager;
  QTreeWidgetItem *layerToggle;
  QWidget *panel;
  QStackedLayout *panelLayout;

  ToolsWindow();

public slots:
  void itemChanged();
};

extern ToolsWindow *toolsWindow;
