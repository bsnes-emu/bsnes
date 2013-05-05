namespace phoenix {

struct pLabel : public pWidget {
  Label& label;

  void setText(string text);

  pLabel(Label& label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
};

}
