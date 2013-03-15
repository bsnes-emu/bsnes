namespace phoenix {

struct pVerticalScroller : public pWidget {
  VerticalScroller &verticalScroller;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller &verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
};

}
