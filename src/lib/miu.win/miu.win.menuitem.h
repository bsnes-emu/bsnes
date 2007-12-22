class pMenuItem : public pMenuControl {
public:
  void create(const char *text = "");

  MenuItem &self;
  pMenuItem(MenuItem&);
};
