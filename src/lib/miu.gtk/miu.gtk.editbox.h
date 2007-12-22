class pEditbox : public pFormControl {
public:
  Editbox &self;
  void create(uint style, uint width, uint height, const char *text = "");
  uint get_text(char *text, uint length = -1U);
  void set_text(const char *text = "");

  pEditbox(Editbox&);

/* internal */
  GtkWidget *scrollbox;
  GtkWidget *editbox;
  GtkTextBuffer *buffer;
  bool multiline;
  GtkWidget* gtk_handle();
};
