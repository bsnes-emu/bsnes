struct InformationWindow : Window {
  VerticalLayout layout;
  TextEdit markup;
  CheckBox markupXML;

  void update();
  void parse(XML::Node &node, string &output, unsigned depth);
  InformationWindow();
};

extern InformationWindow *informationWindow;
