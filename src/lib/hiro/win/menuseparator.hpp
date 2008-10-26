class pMenuSeparator : public pMenuControl {
public:
  MenuSeparator &self;
  void create();

  void enable(bool = true);
  void disable();
  bool enabled();

  pMenuSeparator(MenuSeparator&);
};
