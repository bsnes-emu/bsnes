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
    ComboButton frequency;
    Label latencyLabel;
    ComboButton latency;
    Label resamplerLabel;
    ComboButton resampler;
  AudioSlider volume;

  void synchronize();
  AudioSettings();
};

extern AudioSettings *audioSettings;
