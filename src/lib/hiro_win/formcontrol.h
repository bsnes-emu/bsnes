class pFormControl : public pWidget {
public:
  virtual void resize(uint width, uint height);
  void focus();
  bool focused();
  void enable(bool = true);
  void disable();
  bool enabled();
  uintptr_t handle();

  FormControl &self;
  pFormControl(FormControl&);

  /* internal */
  HWND hwnd;
};
