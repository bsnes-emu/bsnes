struct VideoSlider : HorizontalLayout {
  Label name;
  Label value;
  HorizontalSlider slider;

  VideoSlider();
};

struct VideoSettings : SettingsLayout {
  Label colorAdjustment;
  VideoSlider saturation;
  VideoSlider gamma;
  VideoSlider luminance;
  CheckLabel colorEmulation;
  Label overscanAdjustment;
  VideoSlider overscanHorizontal;
  VideoSlider overscanVertical;

  void synchronize();
  VideoSettings();
};

extern VideoSettings* videoSettings;
