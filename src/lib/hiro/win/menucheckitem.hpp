class pMenuCheckItem : public pMenuControl {
public:
  void create(const char *text = "");
  void check(bool state = true);
  void uncheck();
  bool checked();

  void enable(bool = true);
  void disable();
  bool enabled();

  MenuCheckItem &self;
  pMenuCheckItem(MenuCheckItem&);
};
