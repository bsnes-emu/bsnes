struct Presentation : Window {
  Presentation();
  void showSplash();

  FixedLayout layout;
  Viewport viewport;

  nall::image splash;
};

extern Presentation* presentation;
