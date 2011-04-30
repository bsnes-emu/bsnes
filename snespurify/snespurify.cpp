#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
#include <nall/snes/cartridge.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

static const char applicationTitle[] = "snespurify v10";

struct Application : Window {
  Font font;
  VerticalLayout layout;
  HorizontalLayout pathLayout;
  Label pathLabel;
  LineEdit pathBox;
  Button pathScan;
  Button pathBrowse;
  ListView fileList;
  HorizontalLayout controlLayout;
  Button selectAll;
  Button unselectAll;
  Widget spacer;
  Button fixSelected;

  struct FileInfo {
    string filename;
    string problem;
    string solution;
  };
  linear_vector<FileInfo> fileInfo;
  lstring errors;

  void main();
  void enable(bool);
  void scan();
  void scan(const string &pathname);
  void analyze(const string &filename);
  void repair();
} application;

void Application::main() {
  #if defined(PLATFORM_WIN)
  font.setFamily("Tahoma");
  font.setSize(8);
  #else
  font.setFamily("Sans");
  font.setSize(8);
  #endif
  setTitle(applicationTitle);
  setGeometry({ 128, 128, 600, 360 });
  setWidgetFont(font);

  pathLabel.setText("Path to scan:");
  pathScan.setText("Scan");
  pathBrowse.setText("Browse ...");
  fileList.setHeaderText("Filename", "Problem", "Solution");
  fileList.setHeaderVisible();
  fileList.setCheckable();
  selectAll.setText("Select All");
  unselectAll.setText("Clear All");
  fixSelected.setText("Correct");

  layout.setMargin(5);
  pathLayout.append(pathLabel,       0,  0, 5);
  pathLayout.append(pathBox,        ~0,  0, 5);
  pathLayout.append(pathScan,       80,  0, 5);
  pathLayout.append(pathBrowse,     80,  0   );
  layout.append(pathLayout,                 5);
  layout.append(fileList,           ~0, ~0, 5);
  controlLayout.append(selectAll,   80,  0, 5);
  controlLayout.append(unselectAll, 80,  0, 5);
  controlLayout.append(spacer,      ~0,  0, 5);
  controlLayout.append(fixSelected, 80,  0   );
  layout.append(controlLayout                );
  append(layout);

  onClose = &OS::quit;

  pathBox.onActivate = pathScan.onTick = { &Application::scan, this };

  pathBrowse.onTick = []() {
    string pathname = OS::folderSelect(application, "");
    if(pathname != "") application.pathBox.setText(pathname);
  };

  selectAll.onTick = []() {
    unsigned count = application.fileInfo.size();
    for(unsigned i = 0; i < count; i++) application.fileList.setChecked(i, true);
  };

  unselectAll.onTick = []() {
    unsigned count = application.fileInfo.size();
    for(unsigned i = 0; i < count; i++) application.fileList.setChecked(i, false);
  };

  fixSelected.onTick = { &Application::repair, this };

  setVisible();
}

//don't allow actions to be taken while files are being scanned or fixed
void Application::enable(bool state) {
  if(state == false) {
    setTitle({ applicationTitle, " - working ..." });
  } else {
    setTitle(applicationTitle);
  }

  pathBox.setEnabled(state);
  pathScan.setEnabled(state);
  pathBrowse.setEnabled(state);
  fileList.setEnabled(state);
  selectAll.setEnabled(state);
  unselectAll.setEnabled(state);
  fixSelected.setEnabled(state);
}

void Application::scan() {
  fileInfo.reset();
  fileList.reset();

  string pathname = pathBox.text();
  if(pathname == "") {
    MessageWindow::information(application, "Please specify a directory to scan");
    return;
  }
  pathname.transform("\\", "/");
  if(pathname.endswith("/") == false) pathname.append("/");
  if(directory::exists(pathname) == false) {
    MessageWindow::warning(application, "Specified directory does not exist");
    return;
  }

  enable(false);
  scan(pathname);
  enable(true);

  if(fileInfo.size() == 0) {
    MessageWindow::information(application, "All files are correct");
    return;
  }

  unsigned counter = 0;
  foreach(info, fileInfo) {
    fileList.append(notdir(info.filename), info.problem, info.solution);
    fileList.setChecked(counter++, true);
  }
  fileList.autoSizeColumns();
}

void Application::scan(const string &pathname) {
  lstring files = directory::files(pathname);
  foreach(file, files) {
    OS::processEvents();
    analyze({ pathname, file });
  }

  //recursion
  lstring folders = directory::folders(pathname);
  foreach(folder, folders) scan({ pathname, folder });
}

void Application::analyze(const string &filename) {
  if(file::exists(filename) == false) return;

  if(filename.iendswith(".sfc") || filename.iendswith(".bs")  || filename.iendswith(".st")
  || filename.iendswith(".gb")  || filename.iendswith(".gbc") || filename.iendswith(".sgb")
  || filename.iendswith(".smc") || filename.iendswith(".swc") || filename.iendswith(".fig") || filename.iendswith(".ufo")
  || filename.iendswith(".gd3") || filename.iendswith(".gd7") || filename.iendswith(".dx2") || filename.iendswith(".mgd")
  || filename.iendswith(".mgh") || filename.iendswith(".048") || filename.iendswith(".058") || filename.iendswith(".068")
  || filename.iendswith(".078") || filename.iendswith(".usa") || filename.iendswith(".eur") || filename.iendswith(".jap")
  || filename.iendswith(".aus") || filename.iendswith(".bsx")
  ) {
    filemap map(filename, filemap::mode::read);
    unsigned filesize = map.size();
    SNESCartridge information(map.data(), filesize);

    //note: the ordering of rules is very important
    switch(information.type) {
      case SNESCartridge::TypeNormal:
      case SNESCartridge::TypeBsxSlotted:
      case SNESCartridge::TypeBsxBios:
      case SNESCartridge::TypeSufamiTurboBios:
      case SNESCartridge::TypeSuperGameBoy1Bios:
      case SNESCartridge::TypeSuperGameBoy2Bios: {
        if((filesize & 0x7fff) == 512) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Copier header present";
          info.solution = "Remove copier header";
          fileInfo.append(info);
        }

        if(filename.endswith(".sfc") == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Wrong file extension";
          info.solution = "Rename to .sfc";
          fileInfo.append(info);
        }

        break;
      }

      case SNESCartridge::TypeBsx: {
        if((filesize & 0x7fff) == 512) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Copier header present";
          info.solution = "Remove copier header";
          fileInfo.append(info);
        }

        if(filename.endswith(".bs") == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Wrong file extension";
          info.solution = "Rename to .bs";
          fileInfo.append(info);
        }

        break;
      }

      case SNESCartridge::TypeSufamiTurbo: {
        if((filesize & 0x7fff) == 512) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Copier header present";
          info.solution = "Remove copier header";
          fileInfo.append(info);
        }

        if(filename.endswith(".st") == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Wrong file extension";
          info.solution = "Rename to .st";
          fileInfo.append(info);
        }

        break;
      }

      case SNESCartridge::TypeGameBoy: {
        if(filename.endswith(".gb") == false && filename.endswith(".gbc") == false && filename.endswith(".sgb") == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Wrong file extension";
          info.solution = "Rename to .gb";
          fileInfo.append(info);
        }

        break;
      }
    }
  }
}

void Application::repair() {
  enable(false);
  errors.reset();

  for(unsigned n = 0; n < fileInfo.size(); n++) {
    if(fileList.checked(n) == false) continue;
    OS::processEvents();

    FileInfo &info = fileInfo[n];
    if(info.solution == "Remove copier header") {
      file fp;
      if(fp.open(info.filename, file::mode::read)) {
        unsigned size = fp.size();
        uint8_t *data = new uint8_t[size];
        fp.read(data, size);
        fp.close();
        if(fp.open(info.filename, file::mode::write)) {
          fp.write(data + 512, size - 512);
          fp.close();
        }
      }
    } else if(info.solution == "Rename to .sfc") {
      rename(info.filename, string(nall::basename(info.filename), ".sfc"));
    } else if(info.solution == "Rename to .bs") {
      rename(info.filename, string(nall::basename(info.filename), ".bs"));
    } else if(info.solution == "Rename to .st") {
      rename(info.filename, string(nall::basename(info.filename), ".st"));
    } else if(info.solution == "Rename to .gb") {
      rename(info.filename, string(nall::basename(info.filename), ".gb"));
    }
  }

  if(errors.size() == 0) {
    MessageWindow::information(application, "Selected problems have been corrected");
  } else {
    string output;
    for(unsigned i = 0; i < 3 && i < errors.size(); i++) output.append(string(errors[i], "\n"));
    if(errors.size() > 3) output.append("\n(too many errors to show ...)");
    MessageWindow::information(application, {
      "Selected problems have been corrected, but there were errors:\n\n",
      output
    });
  }

  fileInfo.reset();
  fileList.reset();
  enable(true);
}

int main() {
  application.main();
  OS::main();
  return 0;
}
