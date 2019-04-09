namespace hiro {

struct Settings {
  Settings();
  ~Settings();

  //Windows 8+ draws windows with almost no visible borders
  //to allow resizing the window, the OS places transparent margins around each window
  //this causes AdjustFrameRect and SetWindowPos to hold values larger than the actual window
  //as a result, attempts to call Window::setAlignment(Window) fail to position windows correctly
  //pWindow::setVisible() attempts to compute the actual window bounds to correct Window::frameMargin()
  //note: different window styles have different extended frame bounds
  struct ExtendedFrameBounds {
    uint x = 0;
    uint y = 0;
    uint width = 0;
    uint height = 0;
  };

  //these are the default values for Windows 10 ... they will be updated later if they are incorrect
  ExtendedFrameBounds efbPopup    { 0, 0,  0,  0};
  ExtendedFrameBounds efbFixed    { 2, 0,  4,  2};
  ExtendedFrameBounds efbResizable{10, 0, 20, 10};
};

static Settings settings;

}
