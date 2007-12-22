class pFormControl : public pWidget {
public:
  virtual void resize(uint width, uint height);
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
