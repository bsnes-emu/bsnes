namespace phoenix {

struct pSeparator : public pAction {
  Separator& separator;
  NSMenuItem* cocoaSeparator = nullptr;

  pSeparator(Separator& separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

}
