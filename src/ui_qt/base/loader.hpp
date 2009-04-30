class LoaderWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
    QVBoxLayout *layout;
      QGridLayout *grid;
        QLabel *baseLabel;
        QLineEdit *baseFile;
        QPushButton *baseBrowse;
        QPushButton *baseClear;
        QLabel *slot1Label;
        QLineEdit *slot1File;
        QPushButton *slot1Browse;
        QPushButton *slot1Clear;
        QLabel *slot2Label;
        QLineEdit *slot2File;
        QPushButton *slot2Browse;
        QPushButton *slot2Clear;
        QCheckBox *bypassSuperGameboy;
      QHBoxLayout *controls;
        QPushButton *load;
        QPushButton *cancel;
      QWidget *spacer;

  void setup();
  void syncUi();
  void loadBsxSlottedCartridge(const char*, const char*);
  void loadBsxCartridge(const char*, const char*);
  void loadSufamiTurboCartridge(const char*, const char*, const char*);
  void loadSuperGameboyCartridge(const char*, const char*);

public slots:
  void selectBaseCartridge();
  void clearBaseCartridge();
  void selectSlot1Cartridge();
  void clearSlot1Cartridge();
  void selectSlot2Cartridge();
  void clearSlot2Cartridge();

  void onLoad();
  void onCancel();

private:
  SNES::Cartridge::Mode mode;
  void showWindow(const char *title);
} *winLoader;
