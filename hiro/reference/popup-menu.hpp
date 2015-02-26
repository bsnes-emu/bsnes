namespace phoenix {

struct pPopupMenu : public pObject {
  PopupMenu& popupMenu;

  void append(Action& action);
  void remove(Action& action);
  void setVisible();

  pPopupMenu(PopupMenu& popupMenu) : pObject(popupMenu), popupMenu(popupMenu) {}
  void constructor();
  void destructor();
};

}
