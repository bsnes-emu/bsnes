class pMenuCheckItem : public pMenuControl {
public:
  void create(const char *text = "");
  void check(bool state = true);
  void uncheck();
  bool checked();

  MenuCheckItem &self;
  pMenuCheckItem(MenuCheckItem&);

  /* internal */
  GtkWidget* gtk_handle();
  GtkWidget *item;
  bool locked;
};
