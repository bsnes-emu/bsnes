#if defined(Hiro_BrowserWindow)

namespace hiro {

auto pBrowserWindow::directory(BrowserWindow::State& state) -> string {
  QString directory = QFileDialog::getExistingDirectory(
    state.parent ? state.parent->self()->qtWindow : nullptr,
    state.title ? QString::fromUtf8(state.title) : "Select Directory",
    QString::fromUtf8(state.path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name && name.endsWith("/") == false) name.append("/");
  return name;
}

auto pBrowserWindow::open(BrowserWindow::State& state) -> string {
  string filters;
  for(auto& filter : state.filters) {
    auto part = filter.split("|", 1L);
    if(part.size() != 2) continue;
    filters.append(part[0], " (", part[1].transform(":", " "), ");;");
  }
  filters.trimRight(";;", 1L);

  QString filename = QFileDialog::getOpenFileName(
    state.parent ? state.parent->self()->qtWindow : nullptr,
    state.title ? QString::fromUtf8(state.title) : "Open File",
    QString::fromUtf8(state.path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

auto pBrowserWindow::save(BrowserWindow::State& state) -> string {
  string filters;
  for(auto& filter : state.filters) {
    auto part = filter.split("|", 1L);
    if(part.size() != 2) continue;
    filters.append(part[0], " (", part[1].transform(":", " "), ");;");
  }
  filters.trimRight(";;", 1L);

  QString filename = QFileDialog::getSaveFileName(
    state.parent ? state.parent->self()->qtWindow : nullptr,
    state.title ? QString::fromUtf8(state.title) : "Save File",
    QString::fromUtf8(state.path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

}

#endif
