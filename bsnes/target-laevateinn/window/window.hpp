struct WindowManager {
  struct WindowItem {
    Window *window;
    string name;
    string geometry;
  };
  vector<WindowItem> windowList;
  configuration config;

  void append(Window *window, const string &name);
  void loadGeometry();
  void saveGeometry();
};

extern WindowManager *windowManager;
