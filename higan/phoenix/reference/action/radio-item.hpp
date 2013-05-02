namespace phoenix {

struct pRadioItem : public pAction {
  RadioItem& radioItem;

  bool checked();
  void setChecked();
  void setGroup(const group<RadioItem>& group);
  void setText(const string& text);

  pRadioItem(RadioItem& radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
};

}
