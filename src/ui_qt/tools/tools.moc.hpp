class ToolsWindow : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QTabWidget *tab;

  ToolsWindow();

public slots:
};

extern ToolsWindow *toolsWindow;
