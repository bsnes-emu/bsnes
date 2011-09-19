struct WindowManager {
  struct WindowList {
    Window *window;
    string name;
    string geometry;
  };
  linear_vector<WindowList> windowList;

  void append(Window *window, const string &name);

  string geometry(const Geometry &geometry);
  Geometry geometry(const string &geometry);

  void loadGeometry();
  void saveGeometry();

private:
  configuration config;
};

extern WindowManager *windowManager;
