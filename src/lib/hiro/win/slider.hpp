class pSlider : public pFormControl {
public:
  Slider &self;
  void create(unsigned style, unsigned width, unsigned height, unsigned length);
  unsigned get_position();
  void set_position(unsigned position);

  pSlider(Slider&);

  /* internal */
  unsigned slider_position;
};
