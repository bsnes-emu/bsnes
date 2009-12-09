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
};

extern SettingsWindow *settingsWindow;
