class pListbox : public pFormControl {
public:
  Listbox &self;
  void create(unsigned style, unsigned width, unsigned height, const char *columns = "", const char *text = "");
  void autosize_columns();
  void set_column_width(unsigned column, unsigned width);
  void add_item(const char *text);
  void set_item(unsigned index, const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  pListbox(Listbox&);

  /* internal */
  unsigned column_count;
  bool lostfocus;
};
