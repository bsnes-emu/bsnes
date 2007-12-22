class pButton : public pFormControl {
public:
  Button &self;
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  pButton(Button&);

/* internal */
  GtkWidget *button;
  GtkWidget* gtk_handle();
};
