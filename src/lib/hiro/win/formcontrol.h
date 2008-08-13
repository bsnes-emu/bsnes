class pFormControl : public pWidget {
public:
  virtual void resize(unsigned width, unsigned height);
  void focus();
  bool focused();
  void enable(bool = true);
  void disable();
  bool enabled();
  uintptr_t handle();

  virtual void show(bool = true);
  virtual void hide();
  virtual bool visible();

  FormControl &self;
  pFormControl(FormControl&);

  /* internal */
  HWND hwnd;
};
