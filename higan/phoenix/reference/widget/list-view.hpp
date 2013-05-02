namespace phoenix {

struct pListView : public pWidget {
  ListView& listView;

  void append(const lstring& text);
  void autoSizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring& text);
  void remove(unsigned row);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned row, unsigned column, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned row);

  pListView(ListView& listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
};

}
