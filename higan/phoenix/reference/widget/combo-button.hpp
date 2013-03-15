namespace phoenix {

struct pComboButton : public pWidget {
  ComboButton &comboButton;

  void append(const string &text);
  void modify(unsigned row, const string &text);
  void remove(unsigned row);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboButton(ComboButton &comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
};

}
