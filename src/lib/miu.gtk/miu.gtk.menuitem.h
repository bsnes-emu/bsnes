class pMenuItem : public pMenuControl {
public:
  void create(const char *text = "");

  MenuItem &self;
  pMenuItem(MenuItem&);

/* internal */
  GtkWidget *item;
  GtkWidget* gtk_handle();
};
