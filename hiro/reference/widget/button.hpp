namespace phoenix {

struct pButton : public pWidget {
  Button& button;

  void setBordered(bool bordered);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pButton(Button& button) : pWidget(button), button(button) {}
  void constructor();
  void destructor();
};

}
