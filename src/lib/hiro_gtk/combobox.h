class pCombobox : public pFormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void add_item(const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  Combobox &self;
  pCombobox(Combobox&);

  /* internal */
  GtkWidget *combobox;
  uint counter;
  GtkWidget* gtk_handle();
};
