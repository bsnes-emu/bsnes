namespace phoenix {

struct pComboButton : public pWidget {
  ComboButton& comboButton;

  void append(string text);
  void modify(unsigned row, string text);
  void remove(unsigned row);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboButton(ComboButton& comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
};

}
