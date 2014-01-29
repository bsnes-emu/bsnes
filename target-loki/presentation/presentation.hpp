struct Presentation : Window {
  FixedLayout layout;
  Viewport viewport;
  nall::image splash;

  Presentation();
  void showSplash();
};

extern Presentation* presentation;
