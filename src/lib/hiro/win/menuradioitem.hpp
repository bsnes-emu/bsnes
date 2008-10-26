class pMenuRadioItem : public pMenuControl {
public:
  void create(MenuRadioItemGroup &group, const char *text = "");
  void check();
  bool checked();

  void enable(bool = true);
  void disable();
  bool enabled();

  MenuRadioItem &self;
  pMenuRadioItem(MenuRadioItem&);

  /* internal */
  MenuRadioItemGroup group;
  bool create_checked;
};
