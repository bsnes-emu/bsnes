#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/string.hpp>
#include <nall/ups.hpp>
#include <nall/vector.hpp>
#include <nall/snes/cartridge.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

static const char applicationTitle[] = "snespurify v07";

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
  void createPatch(const string &filename);
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
  pathLayout.append(pathLabel, 80, 0, 5);
  pathLayout.append(pathBox, 0, 0, 5);
  pathLayout.append(pathScan, 80, 0, 5);
  pathLayout.append(pathBrowse, 80, 0);
  layout.append(pathLayout, 0, 25, 5);
  layout.append(fileList, 0, 0, 5);
  controlLayout.append(selectAll, 80, 0, 5);
  controlLayout.append(unselectAll, 80, 0, 5);
  controlLayout.append(spacer, 0, 0, 5);
  controlLayout.append(fixSelected, 80, 0);
  layout.append(controlLayout, 0, 25);
  append(layout);

//  unsigned x = 5, y = 5, width = 600, height = 25;
//  layout.append(pathLabel, x, y, 80, height);
//  layout.append(pathBox, x + 85, y, 335, height);
//  layout.append(pathScan, x + 425, y, 80, height);
//  layout.append(pathBrowse, x + 510, y, 80, height); y += height + 5;
//  layout.append(fileList, x, y, 590, 290); y += 290 + 5;
//  layout.append(selectAll, x, y, 80, height);
//  layout.append(unselectAll, x + 85, y, 80, height);
//  layout.append(fixSelected, 595 - 80, y, 80, height); y += height + 5;
//  setLayout(layout);

  onClose = []() { OS::quit(); };

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

    //the ordering of rules is very important:
    //patches need to be created prior to removal of headers
    //headers need to be removed prior to renaming files (so header removal has correct filename)
    //etc.
    switch(information.type) {
      case SNESCartridge::TypeNormal:
      case SNESCartridge::TypeBsxSlotted:
      case SNESCartridge::TypeBsxBios:
      case SNESCartridge::TypeSufamiTurboBios:
      case SNESCartridge::TypeSuperGameBoy1Bios:
      case SNESCartridge::TypeSuperGameBoy2Bios: {
        string ipsName = { nall::basename(filename), ".ips" };
        string upsName = { nall::basename(filename), ".ups" };
        if(file::exists(ipsName) == true && file::exists(upsName) == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Unsupported patch format";
          info.solution = "Create UPS patch";
          fileInfo.append(info);
        }

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
    if(info.solution == "Create UPS patch") {
      createPatch(info.filename);
    } else if(info.solution == "Remove copier header") {
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

void Application::createPatch(const string &filename) {
  string ipsName = { nall::basename(filename), ".ips" };
  string upsName = { nall::basename(filename), ".ups" };

  file fp;
  if(fp.open(filename, file::mode::read)) {
    unsigned isize = fp.size();
    uint8_t *idata = new uint8_t[isize];
    fp.read(idata, isize);
    fp.close();

    fp.open(ipsName, file::mode::read);
    unsigned psize = fp.size();
    uint8_t *pdata = new uint8_t[psize];
    fp.read(pdata, psize);
    fp.close();

    if(psize >= 8
    && pdata[0] == 'P' && pdata[1] == 'A' && pdata[2] == 'T' && pdata[3] == 'C' && pdata[4] == 'H'
    && pdata[psize - 3] == 'E' && pdata[psize - 2] == 'O' && pdata[psize - 1] == 'F') {
      unsigned osize = 0;
      //no way to determine how big IPS output will be, allocate max size IPS patches support -- 16MB
      uint8_t *odata = new uint8_t[16 * 1024 * 1024]();
      memcpy(odata, idata, isize);

      unsigned offset = 5;
      while(offset < psize - 3) {
        unsigned addr;
        addr  = pdata[offset++] << 16;
        addr |= pdata[offset++] <<  8;
        addr |= pdata[offset++] <<  0;

        unsigned size;
        size  = pdata[offset++] << 8;
        size |= pdata[offset++] << 0;

        if(size == 0) {
          //RLE
          size  = pdata[offset++] << 8;
          size |= pdata[offset++] << 0;

          for(unsigned n = addr; n < addr + size;) {
            odata[n++] = pdata[offset];
            if(n > osize) osize = n;
          }
          offset++;
        } else {
          for(unsigned n = addr; n < addr + size;) {
            odata[n++] = pdata[offset++];
            if(n > osize) osize = n;
          }
        }
      }

      osize = max(isize, osize);
      bool hasHeader = ((isize & 0x7fff) == 512);

      uint8_t *widata = idata;
      unsigned wisize = isize;
      if(hasHeader) {
        //remove copier header for UPS patch creation
        widata += 512;
        wisize -= 512;
      }

      uint8_t *wodata = odata;
      unsigned wosize = osize;
      if(hasHeader) {
        //remove copier header for UPS patch creation
        wodata += 512;
        wosize -= 512;
      }

      ups patcher;
      if(patcher.create(widata, wisize, wodata, wosize, upsName) != ups::result::success) {
        errors.append({ "Failed to create UPS patch: ", upsName, "\n" });
      }

      delete[] odata;
    } else {
      errors.append({ "IPS patch is invalid: ", ipsName, "\n" });
    }

    delete[] idata;
    delete[] pdata;
  }
}

int main() {
  application.main();
  OS::main();
  return 0;
}
