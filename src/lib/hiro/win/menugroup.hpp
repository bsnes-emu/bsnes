class pMenuGroup : public pMenuControl {
public:
  MenuGroup &self;
  void create(const char *text);
  void attach(MenuControl &menucontrol);

  void enable(bool = true);
  void disable();
  bool enabled();

  pMenuGroup(MenuGroup&);

  /* internal */
  HMENU group;
};
