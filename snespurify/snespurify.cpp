#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/foreach.hpp>
#include <nall/platform.hpp>
#include <nall/string.hpp>
#include <nall/ups.hpp>
#include <nall/vector.hpp>
#include <nall/snes/info.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

static const char applicationTitle[] = "snespurify v05";

struct Application : Window {
  Font font;
  Label pathLabel;
  TextBox pathBox;
  Button pathScan;
  Button pathBrowse;
  ListBox fileList;
  Button selectAll;
  Button unselectAll;
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
  font.create("Tahoma", 8);
  #else
  font.create("Sans", 8);
  #endif
  create(128, 128, 600, 360, applicationTitle);
  setDefaultFont(font);

  unsigned x = 5, y = 5, width = 600, height = 25;
  pathLabel.create(*this, x, y, 80, height, "Path to scan:");
  pathBox.create(*this, x + 85, y, 335, height);
  pathScan.create(*this, x + 425, y, 80, height, "Scan");
  pathBrowse.create(*this, x + 510, y, 80, height, "Browse ..."); y += height + 5;
  fileList.create(*this, x, y, 590, 290, "Filename\tProblem\tSolution"); y += 290 + 5;
  selectAll.create(*this, x, y, 80, height, "Select All");
  unselectAll.create(*this, x + 85, y, 80, height, "Clear All");
  fixSelected.create(*this, 595 - 80, y, 80, height, "Correct"); y += height + 5;
  fileList.setHeaderVisible();
  fileList.setCheckable();

  onClose = []() {
    OS::quit();
    return true;
  };

  pathBox.onActivate = pathScan.onTick = { &Application::scan, this };

  pathBrowse.onTick = []() {
    string pathname = OS::folderSelect(application);
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
    fileList.addItem({ notdir(info.filename), "\t", info.problem, "\t", info.solution });
    fileList.setChecked(counter++, true);
  }
  fileList.resizeColumnsToContent();
}

void Application::scan(const string &pathname) {
  lstring files = directory::files(pathname);
  foreach(file, files) {
    OS::run();
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
    snes_information information(map.data(), filesize);

    //the ordering of rules is very important:
    //patches need to be created prior to removal of headers
    //headers need to be removed prior to renaming files (so header removal has correct filename)
    //etc.
    switch(information.type) {
      case snes_information::TypeNormal:
      case snes_information::TypeBsxSlotted:
      case snes_information::TypeBsxBios:
      case snes_information::TypeSufamiTurboBios:
      case snes_information::TypeSuperGameBoy1Bios:
      case snes_information::TypeSuperGameBoy2Bios: {
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

      case snes_information::TypeBsx: {
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

      case snes_information::TypeSufamiTurbo: {
        if(filename.endswith(".st") == false) {
          FileInfo info;
          info.filename = filename;
          info.problem = "Wrong file extension";
          info.solution = "Rename to .st";
          fileInfo.append(info);
        }

        break;
      }

      case snes_information::TypeGameBoy: {
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
    OS::run();

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
