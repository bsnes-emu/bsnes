namespace phoenix {

struct pWidget : public pSizable {
  Widget& widget;

  bool enabled();
  bool focused();
  Size minimumSize();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(string font);
  void setGeometry(const Geometry& geometry);
  void setVisible(bool visible);

  pWidget(Widget& widget) : pSizable(widget), widget(widget) {}
  void constructor();
  void destructor();
};

}
