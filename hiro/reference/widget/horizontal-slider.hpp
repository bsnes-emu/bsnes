namespace phoenix {

struct pHorizontalSlider : public pWidget {
  HorizontalSlider& horizontalSlider;

  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider& horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
};

}
