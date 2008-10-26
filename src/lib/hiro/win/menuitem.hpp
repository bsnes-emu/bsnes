class pMenuItem : public pMenuControl {
public:
  void create(const char *text = "");

  void enable(bool = true);
  void disable();
  bool enabled();

  MenuItem &self;
  pMenuItem(MenuItem&);
};
