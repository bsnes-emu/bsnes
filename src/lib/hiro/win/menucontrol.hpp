class pMenuControl : public pWidget {
public:
  virtual void enable(bool = true);
  virtual void disable();
  virtual bool enabled();

  MenuControl &self;
  pMenuControl(MenuControl&);
  virtual ~pMenuControl();

  /* internal */
  HMENU parent;
  char *text;
};
