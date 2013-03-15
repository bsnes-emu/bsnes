namespace phoenix {

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
};

}
