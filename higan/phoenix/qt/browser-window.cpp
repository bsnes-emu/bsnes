namespace phoenix {

string pBrowserWindow::directory(BrowserWindow::State &state) {
  QString directory = QFileDialog::getExistingDirectory(
    state.parent ? state.parent->p.qtWindow : nullptr,
    state.title ? state.title : "Select Directory",
    QString::fromUtf8(state.path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name && name.endswith("/") == false) name.append("/");
  return name;
}

string pBrowserWindow::open(BrowserWindow::State &state) {
  string filters = state.filters.concatenate(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parentheses = 0;
  for(auto &n : filters) {
    if(n == '(') parentheses++;
    if(n == ')') parentheses--;
    if(n == ',' && parentheses) n = ' ';
  }

  QString filename = QFileDialog::getOpenFileName(
    state.parent ? state.parent->p.qtWindow : nullptr,
    state.title ? state.title : "Open File",
    QString::fromUtf8(state.path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

string pBrowserWindow::save(BrowserWindow::State &state) {
  string filters = state.filters.concatenate(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parentheses = 0;
  for(auto &n : filters) {
    if(n == '(') parentheses++;
    if(n == ')') parentheses--;
    if(n == ',' && parentheses) n = ' ';
  }

  QString filename = QFileDialog::getSaveFileName(
    state.parent ? state.parent->p.qtWindow : nullptr,
    state.title ? state.title : "Save File",
    QString::fromUtf8(state.path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

}
