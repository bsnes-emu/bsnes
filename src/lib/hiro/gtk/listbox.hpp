class pListbox : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height, const char *columns = "", const char *text = "");
  void autosize_columns();
  void set_column_width(unsigned column, unsigned width);
  void add_item(const char *text);
  void set_item(unsigned index, const char *text);
  int  get_selection();
  void set_selection(int index);
  void reset();

  Listbox &self;
  pListbox(Listbox&);

  /* internal */
  GtkWidget *scrollbox;
  GtkWidget *listbox;
  GtkListStore *store;
  struct GtkColumn {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *label;
  };
  vector<GtkColumn> column;
  GtkTreeIter iter;
  int listbox_selection;
  GtkWidget* gtk_handle();
};
