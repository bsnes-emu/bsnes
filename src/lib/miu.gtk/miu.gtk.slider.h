class pSlider : public pFormControl {
public:
  void create(uint style, uint width, uint height, uint length);
  uint get_position();
  void set_position(uint position);

  Slider &self;
  pSlider(Slider&);

/* internal */
  GtkWidget *slider;
  uint slider_position;
  GtkWidget* gtk_handle();
};
