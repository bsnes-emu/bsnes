namespace phoenix {

struct pHorizontalSlider : public pWidget {
  HorizontalSlider& horizontalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider& horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
};

}
