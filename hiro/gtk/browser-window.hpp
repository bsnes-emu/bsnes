namespace hiro {

struct pBrowserWindow {
  static auto directory(BrowserWindow::State& state) -> string;
  static auto open(BrowserWindow::State& state) -> string;
  static auto save(BrowserWindow::State& state) -> string;
};

}
