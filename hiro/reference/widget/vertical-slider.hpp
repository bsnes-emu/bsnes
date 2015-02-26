namespace phoenix {

struct pVerticalSlider : public pWidget {
  VerticalSlider& verticalSlider;

  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider& verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
};

}
