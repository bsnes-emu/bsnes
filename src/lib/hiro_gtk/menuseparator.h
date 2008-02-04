class pMenuSeparator : public pMenuControl {
public:
  MenuSeparator &self;
  void create();

  pMenuSeparator(MenuSeparator&);

  /* internal */
  GtkWidget *item;
  GtkWidget* gtk_handle();
};
