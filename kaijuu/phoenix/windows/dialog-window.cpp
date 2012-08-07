static string FileDialog(bool save, Window &parent, const string &path, const lstring &filter) {
  string dir = path;
  dir.replace("/", "\\");

  string filterList;
  for(auto &filterItem : filter) {
    lstring part;
    part.split("(", filterItem);
    if(part.size() != 2) continue;
    part[1].rtrim<1>(")");
    part[1].replace(" ", "");
    part[1].transform(",", ";");
    filterList.append(string(filterItem, "\t", part[1], "\t"));
  }

  utf16_t wfilter(filterList);
  utf16_t wdir(dir);
  wchar_t wfilename[PATH_MAX + 1] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  if(path.empty() == false) {
    //clear COMDLG32 MRU (most recently used) file list
    //this is required in order for lpstrInitialDir to be honored in Windows 7 and above
    registry::remove("HKCU/Software/Microsoft/Windows/CurrentVersion/Explorer/ComDlg32/LastVisitedPidlMRU/");
    registry::remove("HKCU/Software/Microsoft/Windows/CurrentVersion/Explorer/ComDlg32/OpenSavePidlMRU/");
  }

  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = &parent != &Window::none() ? parent.p.hwnd : 0;
  ofn.lpstrFilter = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile = wfilename;
  ofn.nMaxFile = PATH_MAX;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"";

  bool result = (save == false ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn));
  if(result == false) return "";
  string name = (const char*)utf8_t(wfilename);
  name.transform("\\", "/");
  return name;
}

string pDialogWindow::fileOpen(Window &parent, const string &path, const lstring &filter) {
  return FileDialog(false, parent, path, filter);
}

string pDialogWindow::fileSave(Window &parent, const string &path, const lstring &filter) {
  return FileDialog(true, parent, path, filter);
}

string pDialogWindow::folderSelect(Window &parent, const string &path) {
  wchar_t wfilename[PATH_MAX + 1] = L"";
  BROWSEINFO bi;
  bi.hwndOwner = &parent != &Window::none() ? parent.p.hwnd : 0;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = wfilename;
  bi.lpszTitle = L"";
  bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;
  bool result = false;
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if(pidl) {
    if(SHGetPathFromIDList(pidl, wfilename)) {
      result = true;
      IMalloc *imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc))) {
        imalloc->Free(pidl);
        imalloc->Release();
      }
    }
  }
  if(result == false) return "";
  string name = (const char*)utf8_t(wfilename);
  if(name == "") return "";
  name.transform("\\", "/");
  if(name.endswith("/") == false) name.append("/");
  return name;
}