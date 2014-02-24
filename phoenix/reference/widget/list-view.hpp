namespace phoenix {

struct pListView : public pWidget {
  ListView& listView;

  void append(const lstring& text);
  void autoSizeColumns();
  void remove(unsigned selection);
  void reset();
  void setBackgroundColor(Color color);
  void setCheckable(bool checkable);
  void setChecked(unsigned selection, bool checked);
  void setForegroundColor(Color color);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned selection, unsigned position, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned selection);
  void setText(unsigned selection, unsigned position, string text);

  pListView(ListView& listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
};

}
