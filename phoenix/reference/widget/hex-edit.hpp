namespace phoenix {

struct pHexEdit : public pWidget {
  HexEdit& hexEdit;

  void setBackgroundColor(Color color);
  void setColumns(unsigned columns);
  void setForegroundColor(Color color);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit& hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
};

}
