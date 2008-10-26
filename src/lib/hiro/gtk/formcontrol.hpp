class pFormControl : public pWidget {
public:
  virtual void resize(unsigned width, unsigned height);
  void focus();
  bool focused();
  void enable(bool = true);
  void disable();
  bool enabled();

  FormControl &self;
  pFormControl(FormControl&);

  /* internal */
  virtual GtkWidget* gtk_handle();
};
