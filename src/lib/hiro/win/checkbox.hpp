class pCheckbox : public pFormControl {
public:
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void set_text(const char *text = "");
  void check(bool state = true);
  void uncheck();
  bool checked();

  Checkbox &self;
  pCheckbox(Checkbox&);
};
