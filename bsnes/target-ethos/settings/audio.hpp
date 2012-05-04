struct AudioSlider : HorizontalLayout {
  Label name;
  Label value;
  HorizontalSlider slider;

  AudioSlider();
};

struct AudioSettings : SettingsLayout {
  Label title;
  HorizontalLayout controlLayout;
    Label frequencyLabel;
    ComboBox frequency;
    Label latencyLabel;
    ComboBox latency;
    Label resamplerLabel;
    ComboBox resampler;
  AudioSlider volume;

  void synchronize();
  AudioSettings();
};

extern AudioSettings *audioSettings;
