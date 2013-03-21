namespace phoenix {

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action) : pObject(action), action(action) {}
  void constructor();
  void destructor();
};

}
