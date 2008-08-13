class pButton : public pFormControl {
public:
  Button &self;
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void set_text(const char *text = "");

  pButton(Button&);

  /* internal */
  GtkWidget *button;
  GtkWidget* gtk_handle();
};
