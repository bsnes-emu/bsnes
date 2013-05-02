namespace phoenix {

struct pItem : public pAction {
  Item& item;

  void setImage(const image& image);
  void setText(const string& text);

  pItem(Item& item) : pAction(item), item(item) {}
  void constructor();
  void destructor();
};

}
