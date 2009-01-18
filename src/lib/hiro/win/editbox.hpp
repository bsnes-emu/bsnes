class pEditbox : public pFormControl {
public:
  Editbox &self;
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void resize(unsigned width, unsigned height);
  unsigned get_text(char *text, unsigned length = -1U);
  void set_text(const char *text = "");

  pEditbox(Editbox&);

//private:
  bool autovscroll;
  void update();
};
