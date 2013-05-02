namespace phoenix {

struct pHorizontalScroller : public pWidget {
  HorizontalScroller& horizontalScroller;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScroller(HorizontalScroller& horizontalScroller) : pWidget(horizontalScroller), horizontalScroller(horizontalScroller) {}
  void constructor();
  void destructor();
};

}
