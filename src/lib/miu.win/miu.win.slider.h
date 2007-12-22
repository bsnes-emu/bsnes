class pSlider : public pFormControl {
public:
  Slider &self;
  void create(uint style, uint width, uint height, uint length);
  uint get_position();
  void set_position(uint position);

  pSlider(Slider&);

/* internal */
  uint slider_position;
};
