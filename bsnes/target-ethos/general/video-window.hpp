struct VideoWindow : Window {
  FixedLayout layout;
  Viewport viewport;

  VideoWindow();
};

extern VideoWindow *videoWindow;
