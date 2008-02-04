class pCheckbox : public pFormControl {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");
  void check(bool state = true);
  void uncheck();
  bool checked();

  Checkbox &self;
  pCheckbox(Checkbox&);
};
