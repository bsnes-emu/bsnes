InformationWindow *informationWindow = nullptr;

InformationWindow::InformationWindow() {
  setTitle("Information");

  layout.setMargin(5);
  markup.setFont(application->monospaceFont);
  markupXML.setChecked();
  markupXML.setText("Show original XML markup");

  append(layout);
  layout.append(markup, {~0, ~0}, 5);
  layout.append(markupXML, {~0, 0});

  setGeometry({128, 128, 600, 360});
  windowManager->append(this, "InformationWindow");

  markupXML.onToggle = { &InformationWindow::update, this };
}

void InformationWindow::update() {
  string markupData = interface->markup();
  if(markupXML.checked()) {
    markup.setText(markupData);
    return;
  }

  XML::Document document(markupData);
  markupData = "";
  parse(document, markupData, 0);
  markup.setText(markupData);
}

void InformationWindow::parse(XML::Node &header, string &output, unsigned depth) {
  for(auto &node : header) {
    if(node.name.beginswith("?")) continue;
    for(unsigned n = 0; n < depth; n++) output.append("  ");
    output.append(node.name);
    if(node.children.size() == 0 && !node.data.empty()) output.append(": ", node.data);
    output.append("\n");
    parse(node, output, depth + 1);
  }
}
