namespace phoenix {

struct pLabel : public pWidget {
  Label& label;

  void setText(const string& text);

  pLabel(Label& label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
};

}
