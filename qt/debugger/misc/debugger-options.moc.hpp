class DebuggerOptions : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QCheckBox *cacheUsageBox;

  void synchronize();
  DebuggerOptions();

public slots:
  void toggleCacheUsage();
};

extern DebuggerOptions *debuggerOptions;
