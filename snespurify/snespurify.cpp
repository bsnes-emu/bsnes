#include "snespurify.hpp"

MainWindow mainWindow;

void MainWindow::create() {
  font.setSize(8);
  setWidgetFont(font);
  setTitle("SNES Purify v11");

  navigationLabel.setText("Path to scan:");
  navigationScan.setEnabled(false);
  navigationScan.setText("Scan");
  navigationBrowse.setText("Browse ...");

  listView.setHeaderText("Filename", "Problem");
  listView.setHeaderVisible();
  listView.setCheckable();

  controlSelectAll.setText("Select All");
  controlClearAll.setText("Clear All");
  controlDeleteFiles.setText("Delete ZIP, IPS, UPS files after conversion");
  controlCorrect.setEnabled(false);
  controlCorrect.setText("Correct");

  layout.setMargin(5);
  navigationLayout.append(navigationLabel, 0, 0, 5);
  navigationLayout.append(navigationPath, ~0, 0, 5);
  navigationLayout.append(navigationScan, 80, 0, 5);
  navigationLayout.append(navigationBrowse, 80, 0);
  layout.append(navigationLayout, 5);
  layout.append(listView, ~0, ~0, 5);
  controlLayout.append(controlSelectAll, 80, 0, 5);
  controlLayout.append(controlClearAll, 80, 0, 5);
  controlLayout.append(controlDeleteFiles, 0, 0, 5);
  controlLayout.append(spacer, ~0, 0);
  controlLayout.append(controlCorrect, 80, 0);
  layout.append(controlLayout);
  append(layout);

  onClose = &OS::quit;

  navigationPath.onChange = [this] {
    navigationScan.setEnabled(navigationPath.text() != "");
  };

  navigationPath.onActivate = navigationScan.onTick = { &MainWindow::onScan, this };

  navigationBrowse.onTick = [this] {
    string path = OS::folderSelect(*this, "");
    if(path != "") {
      navigationPath.setText(path);
      navigationScan.setEnabled(true);
      navigationScan.onTick();
    }
  };

  controlSelectAll.onTick = [this] {
    for(unsigned n = 0; n < fileList.size(); n++) listView.setChecked(n, true);
  };

  controlClearAll.onTick = [this] {
    for(unsigned n = 0; n < fileList.size(); n++) listView.setChecked(n, false);
  };

  controlCorrect.onTick = { &MainWindow::onCorrect, this };

  setGeometry({ 128, 128, 640, 360 });
  setVisible();
}

void MainWindow::onScan() {
  string path = navigationPath.text();
  path.transform("\\", "/");
  if(path.endswith("/") == false) path.append("/");

  listView.reset();
  fileList.reset();
  onScanFolder(path);
  foreach(file, fileList) {
    listView.append(notdir(file.name), file.problem);
  }
  controlSelectAll.onTick();
  listView.autoSizeColumns();
  controlCorrect.setEnabled(fileList.size() > 0);
}

void MainWindow::onScanFolder(const string &path) {
  lstring contents = directory::contents(path);
  foreach(filename, contents) {
    if(filename.endswith("/")) onScanFolder({ path, filename });
    else analyzeFile({ path, filename });
  }
}

void MainWindow::analyzeFile(const string &filename) {
  File file;
  file.name = filename;

  if(filename.iendswith(".zip")) {
    //verify there is exactly one SNES image inside the archive
    zip archive;
    if(archive.open(filename)) {
      unsigned fileCount = 0, archiveFileSize = 0;
      string archiveFileName;
      foreach(archiveFile, archive.file) if(isImageName(archiveFile.name)) {
        //make sure file isn't already decompressed
        string basename = { nall::basename(filename), correctExtension(archiveFile.name) };
        if(file::exists(basename)) { fileCount = 0; break; }  //zero fileCount to prevent multi-archive repeated extractions

        fileCount++;
        archiveFileName = archiveFile.name;
        archiveFileSize = archiveFile.size;
      }
      archive.close();
      if(fileCount == 1) {
        file.problem.append("Compressed with ZIP; ");
        if(isSnesImageName(archiveFileName) && (archiveFileSize & 0x7fff) == 512) {
          file.problem.append("Contains copier header; ");
        }
      }
    }
  }

  if(isBadImageName(filename)) {
    file.problem.append("Incorrect extension; ");
  }

  if(isSnesImageName(filename) && (file::size(filename) & 0x7fff) == 512) {
    file.problem.append("Contains copier header; ");
  }

  if((isImageName(filename) || filename.iendswith(".zip")) && !file::exists({ nall::basename(filename), ".bps" })) {
    if(file::exists({ nall::basename(filename), ".ups" })) {
      file.problem.append("Deprecated UPS patch; ");
    } else if(file::exists({ nall::basename(filename), ".ips" })) {
      file.problem.append("Deprecated IPS patch; ");
    }
  }

  file.problem.rtrim<1>("; ");
  if(file.problem != "") fileList.append(file);
}

bool MainWindow::isSnesCartImageName(const string &filename) {
  if(filename.iendswith(".sfc")) return true;
  if(filename.iendswith(".048")) return true;
  if(filename.iendswith(".058")) return true;
  if(filename.iendswith(".068")) return true;
  if(filename.iendswith(".078")) return true;
  if(filename.iendswith(".aus")) return true;
  if(filename.iendswith(".dx2")) return true;
  if(filename.iendswith(".eur")) return true;
  if(filename.iendswith(".fig")) return true;
  if(filename.iendswith(".gd3")) return true;
  if(filename.iendswith(".gd7")) return true;
  if(filename.iendswith(".jap")) return true;
  if(filename.iendswith(".mgd")) return true;
  if(filename.iendswith(".mgh")) return true;
  if(filename.iendswith(".smc")) return true;
  if(filename.iendswith(".swc")) return true;
  if(filename.iendswith(".ufo")) return true;
  if(filename.iendswith(".usa")) return true;
  return false;
}

bool MainWindow::isSnesBsImageName(const string &filename) {
  if(filename.iendswith(".bs")) return true;
  if(filename.iendswith(".bsx")) return true;
  return false;
}

bool MainWindow::isSnesStImageName(const string &filename) {
  if(filename.iendswith(".st")) return true;
  return false;
}

bool MainWindow::isSnesImageName(const string &filename) {
  if(isSnesCartImageName(filename)) return true;
  if(isSnesBsImageName(filename)) return true;
  if(isSnesStImageName(filename)) return true;
  return false;
}

bool MainWindow::isGameBoyClassicImageName(const string &filename) {
  if(filename.iendswith(".gb")) return true;
  if(filename.iendswith(".sgb")) return true;
  return false;
}

bool MainWindow::isGameBoyColorImageName(const string &filename) {
  if(filename.iendswith(".gbc")) return true;
  return false;
}

bool MainWindow::isGameBoyImageName(const string &filename) {
  if(isGameBoyClassicImageName(filename)) return true;
  if(isGameBoyColorImageName(filename)) return true;
  return false;
}

bool MainWindow::isImageName(const string &filename) {
  if(isSnesImageName(filename)) return true;
  if(isGameBoyImageName(filename)) return true;
  return false;
}

bool MainWindow::isGoodImageName(const string &filename) {
  if(filename.endswith(".sfc")) return true;
  if(filename.endswith(".bs")) return true;
  if(filename.endswith(".st")) return true;
  if(filename.endswith(".gb")) return true;
  if(filename.endswith(".gbc")) return true;
  return false;
}

bool MainWindow::isBadImageName(const string &filename) {
  return (isGoodImageName(filename) == false && isImageName(filename) == true);
}

string MainWindow::correctExtension(const string &filename) {
  if(isSnesCartImageName(filename)) return ".sfc";
  if(isSnesBsImageName(filename)) return ".bs";
  if(isSnesStImageName(filename)) return ".st";
  if(isGameBoyClassicImageName(filename)) return ".gb";
  if(isGameBoyColorImageName(filename)) return ".gbc";
  return { ".", extension(filename) };  //should never occur
}

void MainWindow::onCorrect() {
  navigationPath.setEnabled(false);
  navigationScan.setEnabled(false);
  navigationBrowse.setEnabled(false);
  listView.setEnabled(false);
  controlSelectAll.setEnabled(false);
  controlClearAll.setEnabled(false);
  controlDeleteFiles.setEnabled(false);
  controlCorrect.setEnabled(false);
  OS::processEvents();

  MessageWindow::information(*this,
    "Corrections may take a long time to complete.\n"
    "The GUI will be unresponsive during this time, so please be patient.\n"
    "You will be notified when all corrections have been completed."
  );
  OS::processEvents();

  errors.reset();
  foreach(file, fileList, n) {
    if(listView.checked(n) == false) continue;
    if(file.problem.position("Compressed with ZIP")) problemDecompressZip(file);
    if(file.problem.position("Incorrect extension")) problemCorrectExtension(file);
    if(file.problem.position("Deprecated UPS patch")) problemCreatePatchFromUPS(file);
    if(file.problem.position("Deprecated IPS patch")) problemCreatePatchFromIPS(file);
    if(file.problem.position("Contains copier header")) problemRemoveCopierHeader(file);
  }

  if(errors.size() == 0) {
    MessageWindow::information(*this, "Corrections completed!");
  } else {
    file fp;
    if(fp.open({ navigationPath.text(), "errors.txt" }, file::mode::write)) {
      foreach(error, errors) fp.print(error, "\n");
      fp.close();
    }
    MessageWindow::information(*this, {
      "Corrections completed, but there were errors.\n"
      "Please see ", navigationPath.text(), "errors.txt for more details.\n"
    });
  }
  listView.reset();
  listView.autoSizeColumns();
  fileList.reset();

  navigationPath.setEnabled(true);
  navigationScan.setEnabled(true);
  navigationBrowse.setEnabled(true);
  listView.setEnabled(true);
  controlSelectAll.setEnabled(true);
  controlClearAll.setEnabled(true);
  controlDeleteFiles.setEnabled(true);
}

void MainWindow::problemDecompressZip(File &item) {
  zip archive;
  if(archive.open(item.name)) {
    foreach(archiveFile, archive.file) {
      if(isImageName(archiveFile.name)) {
        uint8_t *data;
        unsigned size;
        if(archive.extract(archiveFile, data, size)) {
          string targetFileName = { nall::basename(item.name), correctExtension(archiveFile.name) };
          if(file::write(targetFileName, data, size) == false) {
            errors.append({ "Failed to write file: ", item.name });
          } else {
            if(controlDeleteFiles.checked()) unlink(item.name);
            item.name = targetFileName;
          }
        } else {
          errors.append({ "Failed to decompress file: ", item.name });
        }
        break;
      }
    }
  }
}

void MainWindow::problemCorrectExtension(File &item) {
  string filename = { nall::basename(item.name), correctExtension(item.name) };
  if(rename(item.name, filename) == -1) {
    errors.append({ "Failed to rename file: ", item.name });
  } else {
    item.name = filename;
  }
}

void MainWindow::problemCreatePatchFromUPS(File &item) {
  string upsPatchName = { nall::basename(item.name), ".ups" };
  string bpsPatchName = { nall::basename(item.name), ".bps" };
  ups oldPatch;
  filemap patchFile, sourceFile;
  patchFile.open(upsPatchName, filemap::mode::read);
  sourceFile.open(item.name, filemap::mode::read);
  uint8_t *targetData = 0;
  unsigned targetSize = 0;
  oldPatch.apply(patchFile.data(), patchFile.size(), sourceFile.data(), sourceFile.size(), targetData, targetSize);
  targetData = new uint8_t[targetSize];
  if(oldPatch.apply(patchFile.data(), patchFile.size(), sourceFile.data(), sourceFile.size(), targetData, targetSize)
  == ups::result::success) {
    bpslinear newPatch;
    newPatch.source(item.name);
    newPatch.target(targetData, targetSize);
    if(newPatch.create(bpsPatchName)) {
      patchFile.close();
      if(controlDeleteFiles.checked()) unlink(upsPatchName);
    } else {
      errors.append({ "Failed to create BPS patch: ", bpsPatchName });
    }
  } else {
    errors.append({ "Invalid UPS patch: ", upsPatchName });
  }
}

void MainWindow::problemCreatePatchFromIPS(File &item) {
  string ipsPatchName = { nall::basename(item.name), ".ips" };
  string bpsPatchName = { nall::basename(item.name), ".bps" };
  ips oldPatch;
  oldPatch.source(item.name);
  oldPatch.modify(ipsPatchName);
  if(oldPatch.apply()) {
    bpslinear newPatch;
    newPatch.source(item.name);
    newPatch.target(oldPatch.data, oldPatch.size);
    if(newPatch.create(bpsPatchName)) {
      if(controlDeleteFiles.checked()) unlink(ipsPatchName);
    } else {
      errors.append({ "Failed to create BPS patch: ", bpsPatchName });
    }
  } else {
    errors.append({ "Invalid IPS patch: ", ipsPatchName });
  }
}

/*
void MainWindow::problemCreateUpsPatch(File &item) {
  string ipsPatchName = { nall::basename(item.name), ".ips" };
  ips oldPatch;
  oldPatch.source(item.name);
  oldPatch.modify(ipsPatchName);
  if(oldPatch.apply()) {
    ups newPatch;
    if(newPatch.create(
      oldPatch.sourceData, oldPatch.sourceSize,
      oldPatch.data, oldPatch.size,
      string(nall::basename(item.name), ".ups")
    ) != ups::result::success) {
      errors.append({ "Failed to convert IPS patch: ", ipsPatchName });
    } else {
      if(controlDeleteFiles.checked()) unlink(ipsPatchName);
    }
  }
}
*/

void MainWindow::problemRemoveCopierHeader(File &item) {
  uint8_t *data;
  unsigned size;
  if(file::read(item.name, data, size)) {
    if((size & 0x7fff) == 512) {
      if(file::write(item.name, data + 512, size - 512) == false) {
        errors.append({ "Failed to write file: ", item.name });
      }
    }
  }
}

int main() {
  mainWindow.create();
  OS::main();
  return 0;
}
