namespace phoenix {

struct pCheckLabel : public pWidget {
  CheckLabel& checkLabel;

  void setChecked(bool checked);
  void setText(string text);

  pCheckLabel(CheckLabel& checkLabel) : pWidget(checkLabel), checkLabel(checkLabel) {}
  void constructor();
  void destructor();
};

}
