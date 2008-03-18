class pMenuGroup : public pMenuControl {
public:
  MenuGroup &self;
  void create(const char *text);
  void attach(MenuControl &menucontrol);

  pMenuGroup(MenuGroup&);

  /* internal */
  HMENU group;
};
