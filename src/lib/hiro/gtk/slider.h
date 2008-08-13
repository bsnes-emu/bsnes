class pSlider : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height, unsigned length);
  unsigned get_position();
  void set_position(unsigned position);

  Slider &self;
  pSlider(Slider&);

  /* internal */
  GtkWidget *slider;
  unsigned slider_position;
  GtkWidget* gtk_handle();
};
