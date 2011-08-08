struct AudioSettings {
  HorizontalLayout panelLayout;
  Widget panel;
  VerticalLayout layout;
  Label title;

  HorizontalLayout volumeLayout;
  Label volumeLabel;
  Label volumeValue;
  HorizontalSlider volumeSlider;

  HorizontalLayout frequencyLayout;
  Label frequencyLabel;
  Label frequencyValue;
  HorizontalSlider frequencySlider;

  Widget spacer;

  void create();
};

extern AudioSettings audioSettings;
