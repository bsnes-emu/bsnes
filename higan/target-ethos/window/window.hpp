struct WindowManager {
  struct WindowList {
    Window *window;
    string name;
    string geometry;
  };
  vector<WindowList> windowList;

  void append(Window *window, const string &name);
  void loadGeometry();
  void saveGeometry();
  void hideAll();

private:
  Configuration::Document config;
};

extern WindowManager *windowManager;
