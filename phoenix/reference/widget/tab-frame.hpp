namespace phoenix {

struct pTabFrame : public pWidget {
  TabFrame& tabFrame;

  void append(string text, const image& image);
  void remove(unsigned selection);
  void setImage(unsigned selection, const image& image);
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);

  pTabFrame(TabFrame& tabFrame) : pWidget(tabFrame), tabFrame(tabFrame) {}
  void constructor();
  void destructor();
};

}
