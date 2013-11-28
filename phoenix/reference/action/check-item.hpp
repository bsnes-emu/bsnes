namespace phoenix {

struct pCheckItem : public pAction {
  CheckItem& checkItem;

  void setChecked(bool checked);
  void setText(string text);

  pCheckItem(CheckItem& checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();
};

}
