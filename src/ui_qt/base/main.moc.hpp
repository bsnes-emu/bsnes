class CanvasObject : public QWidget {
public:
  void dragEnterEvent(QDragEnterEvent*);
  void dropEvent(QDropEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
};

class CanvasWidget : public CanvasObject {
public:
  QPaintEngine* paintEngine() const;
  void mouseReleaseEvent(QMouseEvent*);
  void paintEvent(QPaintEvent*);
};

class MainWindow : public QbWindow {
  Q_OBJECT

public:
  QMenuBar *menuBar;
  QStatusBar *statusBar;
  void closeEvent(QCloseEvent*);

  QVBoxLayout *layout;
  QMenu *system;
    QAction *system_load;
    QMenu *system_power;
      QAction *system_power_on;
      QAction *system_power_off;
    QAction *system_reset;
    QMenu *system_port1;
      QAction *system_port1_none;
      QAction *system_port1_joypad;
      QAction *system_port1_multitap;
      QAction *system_port1_mouse;
    QMenu *system_port2;
      QAction *system_port2_none;
      QAction *system_port2_joypad;
      QAction *system_port2_multitap;
      QAction *system_port2_mouse;
      QAction *system_port2_superscope;
      QAction *system_port2_justifier;
      QAction *system_port2_justifiers;
    QAction *system_exit;
  QMenu *settings;
    QMenu *settings_videoMode;
      QAction *settings_videoMode_1x;
      QAction *settings_videoMode_2x;
      QAction *settings_videoMode_3x;
      QAction *settings_videoMode_4x;
      QAction *settings_videoMode_max;
      QAction *settings_videoMode_correctAspectRatio;
      QAction *settings_videoMode_fullscreen;
      QAction *settings_videoMode_ntsc;
      QAction *settings_videoMode_pal;
    QMenu *settings_videoFilter;
      QAction *settings_videoFilter_point;
      QAction *settings_videoFilter_linear;
      QAction *settings_videoFilter_none;
      QAction *settings_videoFilter_scanline;
      QAction *settings_videoFilter_scale2x;
      QAction *settings_videoFilter_lq2x;
      QAction *settings_videoFilter_hq2x;
      QAction *settings_videoFilter_ntsc;
    QAction *settings_muteAudio;
    QMenu *settings_emulationSpeed;
      QAction *settings_emulationSpeed_slowest;
      QAction *settings_emulationSpeed_slow;
      QAction *settings_emulationSpeed_normal;
      QAction *settings_emulationSpeed_fast;
      QAction *settings_emulationSpeed_fastest;
      QAction *settings_emulationSpeed_syncVideo;
      QAction *settings_emulationSpeed_syncAudio;
    QAction *settings_configuration;
  QMenu *tools;
    QAction *tools_cheatEditor;
    QAction *tools_stateManager;
    QAction *tools_debugger;
  QMenu *help;
    QAction *help_documentation;
    QAction *help_license;
    QAction *help_about;
  //
  CanvasObject *canvasContainer;
    QVBoxLayout *canvasLayout;
      CanvasWidget *canvas;
  QLabel *systemState;

  void syncUi();
  bool isActive();
  MainWindow();

public slots:
  void loadCartridge();
  void powerOn();
  void powerOff();
  void reset();
  void setPort1None();
  void setPort1Joypad();
  void setPort1Multitap();
  void setPort1Mouse();
  void setPort2None();
  void setPort2Joypad();
  void setPort2Multitap();
  void setPort2Mouse();
  void setPort2SuperScope();
  void setPort2Justifier();
  void setPort2Justifiers();
  void quit();
  void setVideoMode1x();
  void setVideoMode2x();
  void setVideoMode3x();
  void setVideoMode4x();
  void setVideoModeMax();
  void toggleAspectCorrection();
  void toggleFullscreen();
  void setVideoNtsc();
  void setVideoPal();
  void setPointFilter();
  void setLinearFilter();
  void setNoFilter();
  void setScanlineFilter();
  void setScale2xFilter();
  void setLq2xFilter();
  void setHq2xFilter();
  void setNtscFilter();
  void muteAudio();
  void setSpeedSlowest();
  void setSpeedSlow();
  void setSpeedNormal();
  void setSpeedFast();
  void setSpeedFastest();
  void syncVideo();
  void syncAudio();
  void showConfigWindow();
  void showCheatEditor();
  void showStateManager();
  void showDebugger();
  void showDocumentation();
  void showLicense();
  void showAbout();
} *mainWindow;
