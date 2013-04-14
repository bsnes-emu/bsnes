namespace phoenix {

struct pRadioButton : public pWidget {
  RadioButton &radioButton;

  bool checked();
  void setChecked();
  void setGroup(const group<RadioButton&> &group);
  void setText(const string &text);

  pRadioButton(RadioButton &radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  void constructor();
  void destructor();
};

}
