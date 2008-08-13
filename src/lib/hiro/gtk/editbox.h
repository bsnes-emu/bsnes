class pEditbox : public pFormControl {
public:
  Editbox &self;
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  unsigned get_text(char *text, unsigned length = -1U);
  void set_text(const char *text = "");

  pEditbox(Editbox&);

  /* internal */
  GtkWidget *scrollbox;
  GtkWidget *editbox;
  GtkTextBuffer *buffer;
  bool multiline;
  GtkWidget* gtk_handle();
};
