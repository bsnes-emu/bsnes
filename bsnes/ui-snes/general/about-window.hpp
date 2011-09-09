struct AboutWindow : TopLevelWindow {
  VerticalLayout layout;
  Canvas canvas;
  HorizontalLayout informationLayout;
  Widget spacer;
  Label information;

  void create();
  void show();

private:
  resource logo;
};

extern AboutWindow aboutWindow;
