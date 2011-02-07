struct AudioSettings : TopLevelWindow {
  VerticalLayout layout;
  HorizontalLayout volumeLayout;
  Label volumeLabel;
  Label volumeValue;
  HorizontalSlider volumeSlider;
  HorizontalLayout frequencyLayout;
  Label frequencyLabel;
  Label frequencyValue;
  HorizontalSlider frequencySlider;

  void create();
};

extern AudioSettings audioSettings;
