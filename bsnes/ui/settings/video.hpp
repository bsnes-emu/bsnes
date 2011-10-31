struct VideoSlider : HorizontalLayout {
  Label name;
  Label value;
  HorizontalSlider slider;

  VideoSlider();
};

struct VideoSettings : SettingsLayout {
  Label title;
  Label colorAdjustment;
  VideoSlider brightness;
  VideoSlider contrast;
  VideoSlider gamma;
  Label overscanAdjustment;
  VideoSlider overscanHorizontal;
  VideoSlider overscanVertical;
  Label fullScreenMode;
  HorizontalLayout fullScreenLayout;
  RadioBox fullScreen[3];
  Label compositorLabel;
  HorizontalLayout compositorLayout;
  RadioBox compositor[3];

  void synchronize();
  VideoSettings();
};

extern VideoSettings *videoSettings;
