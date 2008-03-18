class pFrame : public pFormControl {
public:
  Frame &self;
  void create(uint style, uint width, uint height, const char *text = "");
  void set_text(const char *text = "");

  pFrame(Frame&);
};
