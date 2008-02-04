class pListbox : public pFormControl {
public:
  void create(uint style, uint width, uint height, const char *columns = "", const char *text = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *text);
  void set_item(uint index, const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  Listbox &self;
  pListbox(Listbox&);

  /* internal */
  GtkWidget *scrollbox;
  GtkWidget *listbox;
  GtkListStore *store;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  array<GtkTreeViewColumn*> column_list;
  GtkTreeIter iter;
  int listbox_selection;
  GtkWidget* gtk_handle();
};
