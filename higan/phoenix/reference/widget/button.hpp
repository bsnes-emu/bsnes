namespace phoenix {

struct pButton : public pWidget {
  Button &button;

  void setImage(const image &image, Orientation orientation);
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();
};

}
