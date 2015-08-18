#if defined(Hiro_BrowserWindow)

namespace hiro {

auto pBrowserWindow::directory(BrowserWindow::State& state) -> string {
/*
  QString directory = QFileDialog::getExistingDirectory(
    state.parent ? state.parent->p.qtWindow : nullptr,
    state.title ? state.title : "Select Directory",
    QString::fromUtf8(state.path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name && name.endsWith("/") == false) name.append("/");
  return name;
*/
}

auto pBrowserWindow::open(BrowserWindow::State& state) -> string {
/*
  string filters = state.filters.merge(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parentheses = 0;
  for(auto& n : filters) {
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
*/
}

auto pBrowserWindow::save(BrowserWindow::State& state) -> string {
/*
  string filters = state.filters.merge(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parentheses = 0;
  for(auto& n : filters) {
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
*/
}

}

#endif
