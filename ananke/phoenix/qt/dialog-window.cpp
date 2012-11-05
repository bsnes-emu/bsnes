string pDialogWindow::fileOpen(Window &parent, const string &path, const lstring &filter) {
  string filterList;
  for(auto &item : filter) {
    filterList.append(item);
    filterList.append(";;");
  }
  filterList.rtrim<1>(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parenthesis = 0;
  for(auto &n : filterList) {
    if(n == '(') parenthesis++;
    if(n == ')') parenthesis--;
    if(n == ',' && parenthesis) n = ' ';
  }

  QString filename = QFileDialog::getOpenFileName(
    &parent != &Window::none() ? parent.p.qtWindow : nullptr, "Open File",
    QString::fromUtf8(path), QString::fromUtf8(filterList)
  );
  return filename.toUtf8().constData();
}

string pDialogWindow::fileSave(Window &parent, const string &path, const lstring &filter) {
  string filterList;
  for(auto &item : filter) {
    filterList.append(item);
    filterList.append(";;");
  }
  filterList.rtrim<1>(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parenthesis = 0;
  for(auto &n : filterList) {
    if(n == '(') parenthesis++;
    if(n == ')') parenthesis--;
    if(n == ',' && parenthesis) n = ' ';
  }

  QString filename = QFileDialog::getSaveFileName(
    &parent != &Window::none() ? parent.p.qtWindow : nullptr, "Save File",
    QString::fromUtf8(path), QString::fromUtf8(filterList)
  );
  return filename.toUtf8().constData();
}

string pDialogWindow::folderSelect(Window &parent, const string &path) {
  QString directory = QFileDialog::getExistingDirectory(
    &parent != &Window::none() ? parent.p.qtWindow : nullptr, "Select Directory",
    QString::fromUtf8(path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name != "" && name.endswith("/") == false) name.append("/");
  return name;
}
