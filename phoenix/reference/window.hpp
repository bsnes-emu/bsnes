namespace phoenix {

struct pWindow : public pObject {
  Window& window;

  static Window& none();

  void append(Layout& layout);
  void append(Menu& menu);
  void append(Widget& widget);
  Color backgroundColor();
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout& layout);
  void remove(Menu& menu);
  void remove(Widget& widget);
  void setBackgroundColor(Color color);
  void setDroppable(bool droppable);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(Geometry geometry);
  void setMenuFont(string font);
  void setMenuVisible(bool visible);
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(string font);
  void setStatusText(string text);
  void setStatusVisible(bool visible);
  void setTitle(string text);
  void setVisible(bool visible);
  void setWidgetFont(string font);

  pWindow(Window& window) : pObject(window), window(window) {}
  void constructor();
  void destructor();
};

}
