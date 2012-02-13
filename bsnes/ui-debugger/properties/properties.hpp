struct PropertiesViewer : Window {
  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Label sourceLabel;
      ComboBox sourceSelection;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    TextEdit properties;

  void updateProperties();
  PropertiesViewer();
};

extern PropertiesViewer *propertiesViewer;
