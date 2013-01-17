#include <nall/nall.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <thread>

#include "resource/resource.cpp"

struct Application : Window {
  library ananke;

  VerticalLayout layout;
    HorizontalLayout pathLayout;
      Label pathLabel;
      LineEdit pathEdit;
      Button browseButton;
    ListView fileList;
    ProgressBar progressBar;
    HorizontalLayout controlLayout;
      Button selectAllButton;
      Button unselectAllButton;
      Widget spacer;
      Button purifyButton;

  lstring filenameList;

  Application();
  void scanPath();
  void scanPath(const string &path, const string &basepath);
  void purify();
} *application = nullptr;

Application::Application() {
  application = this;

  if(ananke.open("ananke") == false) {
    MessageWindow::critical(Window::none(),
      "Error: ananke was not found, but is required to use purify.\n\n"
      "Please install ananke and then run purify again."
    );
    exit(0);
  }

  setFrameGeometry({64, 64, 720, 480});
  setTitle("purify v02");

  layout.setMargin(5);
  pathLabel.setText("Path:");
  browseButton.setText("Browse ...");
  fileList.setCheckable(true);
  selectAllButton.setText("Select All");
  unselectAllButton.setText("Unselect All");
  purifyButton.setText("Purify");

  append(layout);
  layout.append(pathLayout, {~0, 0}, 5);
    pathLayout.append(pathLabel, {0, 0}, 5);
    pathLayout.append(pathEdit, {~0, 0}, 5);
    pathLayout.append(browseButton, {80, 0});
  layout.append(fileList, {~0, ~0}, 5);
  layout.append(progressBar, {~0, 0}, 5);
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(selectAllButton, {100, 0}, 5);
    controlLayout.append(unselectAllButton, {100, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(purifyButton, {80, 0});

  setVisible();

  onClose = &OS::quit;

  pathEdit.onActivate = {&Application::scanPath, this};

  browseButton.onActivate = [&] {
    string path = DialogWindow::folderSelect(*this, userpath());
    if(path.empty() == false) {
      pathEdit.setText(path);
      scanPath();
    }
  };

  selectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < filenameList.size(); n++) fileList.setChecked(n, true);
  };

  unselectAllButton.onActivate = [&] {
    for(unsigned n = 0; n < filenameList.size(); n++) fileList.setChecked(n, false);
  };

  purifyButton.onActivate = {&Application::purify, this};
}

void Application::scanPath() {
  string path = pathEdit.text();
  fileList.reset();
  filenameList.reset();
  scanPath(path, path);
  selectAllButton.onActivate();
}

void Application::scanPath(const string &path, const string &basepath) {
  lstring files = directory::icontents(path);
  for(auto &file : files) {
    if(
      directory::exists({path, file})
    && !file.endswith(".fc/")
    && !file.endswith(".sfc/")
    && !file.endswith(".st/")
    && !file.endswith(".bs/")
    && !file.endswith(".gb/")
    && !file.endswith(".gbc/")
    && !file.endswith(".gba/")
    ) {
      scanPath({path, file}, basepath);
    } else if(
       directory::exists({path, file})
    ) {
      fileList.append(string{path, file}.ltrim<1>(basepath).rtrim<1>("/"));
      filenameList.append({path, file});
      fileList.setImage(filenameList.size() - 1, 0, {resource::game, sizeof resource::game});
    } else if(
       file.endswith(".fc") || file.endswith(".nes")
    || file.endswith(".sfc") || file.endswith(".smc")
    || file.endswith(".st") || file.endswith(".bs")
    || file.endswith(".gb")
    || file.endswith(".gbc")
    || file.endswith(".gba")
    || file.endswith(".bpa") || file.endswith(".zip")
    ) {
      fileList.append(string{path, file}.ltrim<1>(basepath));
      filenameList.append({path, file});
      if(file.endswith(".bpa") || file.endswith(".zip")) {
        fileList.setImage(filenameList.size() - 1, 0, {resource::archive, sizeof resource::archive});
      } else {
        fileList.setImage(filenameList.size() - 1, 0, {resource::file, sizeof resource::file});
      }
    }
  }
}

struct PurifyContext {
  lstring list;
  unsigned position;
  unsigned size;

  void run() {
    function<string (string)> sync = application->ananke.sym("ananke_sync");
    function<string (string)> open = application->ananke.sym("ananke_open");

    if(!open || !sync) {
      position = size;
      return;
    }

    while(position < size) {
      string filename = list[position];
      if(directory::exists(filename)) sync(filename);
      else if(file::exists(filename)) open(filename);
      position++;
    }
  }

  PurifyContext(const lstring &list) : list(list) {
    position = 0;
    size = list.size();
  }
};

void Application::purify() {
  lstring purifyList;
  for(unsigned n = 0; n < filenameList.size(); n++) {
    if(fileList.checked(n)) purifyList.append(filenameList[n]);
  }

  if(purifyList.size() == 0) {
    MessageWindow::information(*this, "Please select at least one file to purify.");
    return;
  }

  layout.setEnabled(false);
  OS::processEvents();

  PurifyContext purifyContext(purifyList);
  std::thread purifyThread([&] { purifyContext.run(); });
  while(purifyContext.position < purifyContext.size) {
    OS::processEvents();
    unsigned position = ((unsigned)(double)purifyContext.position / (double)purifyContext.size * 100.0 + 0.5);
    progressBar.setPosition(position);
  }
  purifyThread.join();

  fileList.reset();
  filenameList.reset();
  progressBar.setPosition(0);
  layout.setEnabled(true);
}

int main(int argc, char **argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  new Application;
  OS::main();

  return 0;
}
