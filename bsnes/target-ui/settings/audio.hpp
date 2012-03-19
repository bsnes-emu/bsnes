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
  Label outputLabel;
  HorizontalLayout outputLayout;
    Label frequencyLabel;
    ComboBox frequencySelection;
    Label latencyLabel;
    ComboBox latencySelection;
    Label resamplerLabel;
    ComboBox resamplerSelection;
  AudioSlider volume;
  Label frequencyAdjustmentLabel;
  AudioSlider nes;
  AudioSlider snes;
  AudioSlider gb;

  void synchronize();
  AudioSettings();
};

extern AudioSettings *audioSettings;
