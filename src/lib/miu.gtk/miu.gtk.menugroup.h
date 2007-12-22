class pMenuGroup : public pMenuControl {
public:
  MenuGroup &self;
  void create(const char *text);
  void attach(MenuControl &menucontrol);

  pMenuGroup(MenuGroup&);

/* internal */
  GtkWidget *group;
  GtkWidget *item;
  GtkWidget* gtk_handle();
};
