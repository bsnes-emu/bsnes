class LayerToggleWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *title;
  QGridLayout *gridLayout;
  QLabel *bg1Label;
  QCheckBox *bg1pri0;
  QCheckBox *bg1pri1;
  QLabel *bg2Label;
  QCheckBox *bg2pri0;
  QCheckBox *bg2pri1;
  QLabel *bg3Label;
  QCheckBox *bg3pri0;
  QCheckBox *bg3pri1;
  QLabel *bg4Label;
  QCheckBox *bg4pri0;
  QCheckBox *bg4pri1;
  QLabel *oamLabel;
  QCheckBox *oampri0;
  QCheckBox *oampri1;
  QCheckBox *oampri2;
  QCheckBox *oampri3;
  QWidget *spacer;

  LayerToggleWindow();

public slots:
  void stateChanged();
};

extern LayerToggleWindow *layerToggleWindow;
