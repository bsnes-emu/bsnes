namespace phoenix {

struct pCheckButton : public pWidget {
  CheckButton& checkButton;

  void setChecked(bool checked);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pCheckButton(CheckButton& checkButton) : pWidget(checkButton), checkButton(checkButton) {}
  void constructor();
  void destructor();
};

}
