namespace phoenix {

struct pSeparator : public pAction {
  Separator &separator;
  NSMenuItem *cocoaSeparator;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

}
