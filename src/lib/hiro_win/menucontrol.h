class pMenuControl : public pWidget {
public:
  void enable(bool = true);
  void disable();
  bool enabled();

  MenuControl &self;
  pMenuControl(MenuControl&);
  virtual ~pMenuControl();

  /* internal */
  HMENU parent;
  char *text;
};
