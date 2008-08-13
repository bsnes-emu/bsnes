class pCombobox : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void add_item(const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  Combobox &self;
  pCombobox(Combobox&);

  /* internal */
  GtkWidget *combobox;
  unsigned counter;
  GtkWidget* gtk_handle();
};
