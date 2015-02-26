namespace phoenix {

struct pTabFrame : public pWidget {
  TabFrame& tabFrame;

  void append();
  void remove(unsigned selection);
  void setImage(unsigned selection, const image& image);
  void setSelected(unsigned selection);
  void setText(unsigned selection, string text);

  pTabFrame(TabFrame& tabFrame) : pWidget(tabFrame), tabFrame(tabFrame) {}
  void constructor();
  void destructor();
};

}
