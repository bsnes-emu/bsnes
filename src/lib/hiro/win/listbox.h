class pListbox : public pFormControl {
public:
  Listbox &self;
  void create(uint style, uint width, uint height, const char *columns = "", const char *text = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *text);
  void set_item(uint index, const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  pListbox(Listbox&);

  /* internal */
  uint column_count;
};
