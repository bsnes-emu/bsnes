class pMenuControl : public pWidget {
public:
  void enable(bool = true);
  void disable();
  bool enabled();

  MenuControl &self;
  pMenuControl(MenuControl&);

  /* internal */
  virtual GtkWidget* gtk_handle();
};
