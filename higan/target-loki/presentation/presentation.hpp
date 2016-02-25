struct Presentation : Window {
  Presentation();
  auto drawSplashScreen() -> void;

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};
};

extern unique_pointer<Presentation> presentation;
