struct AudioSlider : HorizontalLayout {
  Label name;
  Label value;
  HorizontalSlider slider;

  unsigned base;
  unsigned step;

  unsigned position();
  void setPosition(unsigned position);
  AudioSlider();
};

struct AudioSettings : SettingsLayout {
  Label title;
  Label frequencyAdjustmentLabel;
  AudioSlider nes;
  AudioSlider snes;
  AudioSlider gameBoy;
  Label outputAdjustmentLabel;
  AudioSlider volume;

  void synchronize();
  AudioSettings();
};

extern AudioSettings *audioSettings;
