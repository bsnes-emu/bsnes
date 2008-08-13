class pLabel : public pFormControl {
public:
  Label &self;
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void set_text(const char *text = "");

  pLabel(Label&);

  /* internal */
  GtkWidget *label;
  GtkWidget* gtk_handle();
};
