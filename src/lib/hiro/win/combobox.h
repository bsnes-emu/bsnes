class pCombobox : public pFormControl {
public:
  Combobox &self;
  void create(uint style, uint width, uint height, const char *text = "");
  void add_item(const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  pCombobox(Combobox&);

  /* internal */
  int combobox_selection;
};
