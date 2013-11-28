namespace phoenix {

struct pVerticalScroller : public pWidget {
  VerticalScroller& verticalScroller;

  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller& verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
};

}
