namespace phoenix {

struct pMenu : public pAction {
  Menu& menu;

  void append(Action& action);
  void remove(Action& action);
  void setImage(const image& image);
  void setText(const string& text);

  pMenu(Menu& menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
};

}
