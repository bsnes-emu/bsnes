class pMenuRadioItem : public pMenuControl {
public:
  void create(MenuRadioItemGroup &group, const char *text = "");
  void check();
  bool checked();

  MenuRadioItem &self;
  pMenuRadioItem(MenuRadioItem&);

  /* internal */
  GtkWidget* gtk_handle();
  GtkWidget *item;
  bool locked;
};
