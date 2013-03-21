namespace phoenix {

struct pWindow : public pObject {
  Window &window;

  static Window& none();

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Color backgroundColor();
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout &layout);
  void remove(Menu &menu);
  void remove(Widget &widget);
  void setBackgroundColor(const Color &color);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setMenuFont(const string &font);
  void setMenuVisible(bool visible);
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(const string &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(const string &font);

  pWindow(Window &window) : pObject(window), window(window) {}
  void constructor();
  void destructor();
};

}
