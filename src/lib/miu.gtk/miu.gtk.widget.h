class pWidget {
public:
  void show(bool = true);
  void hide();
  bool visible();
  uintptr_t handle();

  Widget &self;
  pWidget(Widget&);

/* internal */
  virtual GtkWidget* gtk_handle();
};
