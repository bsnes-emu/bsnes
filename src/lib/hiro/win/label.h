class pLabel : public pFormControl {
public:
  Label &self;
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  pLabel(Label&);
};
