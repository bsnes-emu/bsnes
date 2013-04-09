namespace phoenix {

struct pWidget : public pSizable {
  Widget &widget;
  NSView *cocoaView = nullptr;

  bool enabled();
  bool focused();
  virtual Size minimumSize();
  void setEnabled(bool enabled);
  void setFocused();
  virtual void setFont(const string &font);
  virtual void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : pSizable(widget), widget(widget) {}
  void constructor();
  void destructor();
};

}
