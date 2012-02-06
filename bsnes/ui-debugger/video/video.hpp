struct VideoWindow : Window {
  VerticalLayout layout;
  Canvas canvas;

  VideoWindow();
};

extern VideoWindow *videoWindow;
