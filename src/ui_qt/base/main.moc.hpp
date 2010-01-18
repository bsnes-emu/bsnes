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

class MainWindow : public Window {
  Q_OBJECT

public:
  QMenuBar *menuBar;
  QStatusBar *statusBar;
  QVBoxLayout *layout;
  QMenu *system;
    QAction *system_load;
    QMenu *system_loadSpecial;
      QAction *system_loadSpecial_bsxSlotted;
      QAction *system_loadSpecial_bsx;
      QAction *system_loadSpecial_sufamiTurbo;
      QAction *system_loadSpecial_superGameBoy;
    CheckAction *system_power;
    QAction *system_reset;
    QMenu *system_port1;
      RadioAction *system_port1_none;
      RadioAction *system_port1_gamepad;
      RadioAction *system_port1_asciipad;
      RadioAction *system_port1_multitap;
      RadioAction *system_port1_mouse;
    QMenu *system_port2;
      RadioAction *system_port2_none;
      RadioAction *system_port2_gamepad;
      RadioAction *system_port2_asciipad;
      RadioAction *system_port2_multitap;
      RadioAction *system_port2_mouse;
      RadioAction *system_port2_superscope;
      RadioAction *system_port2_justifier;
      RadioAction *system_port2_justifiers;
    QAction *system_exit;
  QMenu *settings;
    QMenu *settings_videoMode;
      RadioAction *settings_videoMode_1x;
      RadioAction *settings_videoMode_2x;
      RadioAction *settings_videoMode_3x;
      RadioAction *settings_videoMode_4x;
      RadioAction *settings_videoMode_5x;
      CheckAction *settings_videoMode_correctAspectRatio;
      RadioAction *settings_videoMode_ntsc;
      RadioAction *settings_videoMode_pal;
    QMenu *settings_videoFilter;
      QAction *settings_videoFilter_configure;
      RadioAction *settings_videoFilter_none;
      array<RadioAction*> settings_videoFilter_list;
    CheckAction *settings_smoothVideo;
    CheckAction *settings_muteAudio;
    QMenu *settings_emulationSpeed;
      RadioAction *settings_emulationSpeed_slowest;
      RadioAction *settings_emulationSpeed_slow;
      RadioAction *settings_emulationSpeed_normal;
      RadioAction *settings_emulationSpeed_fast;
      RadioAction *settings_emulationSpeed_fastest;
      CheckAction *settings_emulationSpeed_syncVideo;
      CheckAction *settings_emulationSpeed_syncAudio;
    QAction *settings_configuration;
  QMenu *tools;
    QMenu *tools_movies;
      QAction *tools_movies_play;
      QAction *tools_movies_stop;
      QAction *tools_movies_recordFromPowerOn;
      QAction *tools_movies_recordFromHere;
    QAction *tools_captureScreenshot;
    QAction *tools_cheatEditor;
    QAction *tools_cheatFinder;
    QAction *tools_stateManager;
    QAction *tools_debugger;
  QMenu *help;
    QAction *help_documentation;
    QAction *help_license;
    QAction *help_about;

  CanvasObject *canvasContainer;
  QVBoxLayout *canvasLayout;
  CanvasWidget *canvas;
  QLabel *systemState;

  void syncUi();
  bool isActive();
  void closeEvent(QCloseEvent*);
  MainWindow();

public slots:
  void loadCartridge();
  void loadBsxSlottedCartridge();
  void loadBsxCartridge();
  void loadSufamiTurboCartridge();
  void loadSuperGameBoyCartridge();
  void power();
  void reset();
  void setPort1None();
  void setPort1Gamepad();
  void setPort1Asciipad();
  void setPort1Multitap();
  void setPort1Mouse();
  void setPort2None();
  void setPort2Gamepad();
  void setPort2Asciipad();
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
  void setVideoMode5x();
  void toggleAspectCorrection();
  void setVideoNtsc();
  void setVideoPal();
  void configureFilter();
  void setFilter();
  void toggleSmoothVideo();
  void muteAudio();
  void setSpeedSlowest();
  void setSpeedSlow();
  void setSpeedNormal();
  void setSpeedFast();
  void setSpeedFastest();
  void syncVideo();
  void syncAudio();
  void showConfigWindow();
  void playMovie();
  void stopMovie();
  void recordMovieFromPowerOn();
  void recordMovieFromHere();
  void saveScreenshot();
  void showCheatEditor();
  void showCheatFinder();
  void showStateManager();
  void showDebugger();
  void showDocumentation();
  void showLicense();
  void showAbout();
};

extern MainWindow *mainWindow;
