class pRadiobox : public pFormControl {
public:
  void create(RadioboxGroup &group, uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check();
  bool checked();

  Radiobox &self;
  pRadiobox(Radiobox&);

  /* internal */
  GtkWidget* gtk_handle();
  GtkWidget *radiobox;
  bool locked;
};
