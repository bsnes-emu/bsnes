class EffectToggleWindow : public QWidget {
  Q_OBJECT

public:
  QGridLayout *layout;
  QLabel *ppuLabel;
  QCheckBox *bg1pri0;
  QCheckBox *bg1pri1;
  QCheckBox *bg2pri0;
  QCheckBox *bg2pri1;
  QCheckBox *bg3pri0;
  QCheckBox *bg3pri1;
  QCheckBox *bg4pri0;
  QCheckBox *bg4pri1;
  QCheckBox *oampri0;
  QCheckBox *oampri1;
  QCheckBox *oampri2;
  QCheckBox *oampri3;
  QLabel *dspLabel;
  QCheckBox *channel0;
  QCheckBox *channel1;
  QCheckBox *channel2;
  QCheckBox *channel3;
  QCheckBox *channel4;
  QCheckBox *channel5;
  QCheckBox *channel6;
  QCheckBox *channel7;

  EffectToggleWindow();

public slots:
  void synchronize();
};

extern EffectToggleWindow *effectToggleWindow;
