class pFrame : public pFormControl {
public:
  Frame &self;
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void set_text(const char *text = "");

  pFrame(Frame&);

  /* internal */
  GtkWidget *frame;
  GtkWidget* gtk_handle();
};
