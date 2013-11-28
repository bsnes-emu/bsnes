namespace phoenix {

struct pRadioItem : public pAction {
  RadioItem& radioItem;

  void setChecked();
  void setGroup(const group<RadioItem>& group);
  void setText(string text);

  pRadioItem(RadioItem& radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
};

}
