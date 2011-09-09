struct AudioSettings {
  HorizontalLayout panelLayout;
  Widget panel;
  VerticalLayout layout;
  Label title;

  HorizontalLayout frequencyLayout;
  Label frequencyLabel;
  Label frequencyValue;
  HorizontalSlider frequencySlider;

  HorizontalLayout volumeLayout;
  Label volumeLabel;
  Label volumeValue;
  HorizontalSlider volumeSlider;

  HorizontalLayout balanceLayout;
  Label balanceLabel;
  Label balanceValue;
  HorizontalSlider balanceSlider;

  Widget spacer;

  void create();
};

extern AudioSettings audioSettings;
