struct VRAMViewer : Window {
  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Label modeLabel;
      ComboBox modeSelection;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    Canvas canvas;

  void updateTiles();
  VRAMViewer();
};

extern VRAMViewer *vramViewer;
