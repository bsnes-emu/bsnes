namespace phoenix {

static int CALLBACK BrowserWindowCallbackProc(HWND hwnd, UINT msg, LPARAM lparam, LPARAM lpdata) {
  if(msg == BFFM_INITIALIZED) {
    if(lpdata) {
      auto state = (BrowserWindow::State*)lpdata;
      utf16_t wpath(string{state->path}.transform("/", "\\"));
      if(state->title) SetWindowText(hwnd, utf16_t(state->title));
      SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(wchar_t*)wpath);
    }
  }
  return 0;
}

static string BrowserWindow_fileDialog(bool save, BrowserWindow::State &state) {
  string path = string{path}.replace("/", "\\");

  string filters;
  for(auto &filter : state.filters) {
    lstring part = filter.split("(");
    if(part.size() != 2) continue;
    part[1].rtrim<1>(")");
    part[1].replace(" ", "");
    part[1].transform(",", ";");
    filters.append(filter, "\t", part[1], "\t");
  }

  utf16_t wfilters(filters);
  wchar_t wname[PATH_MAX + 1] = L"";
  utf16_t wpath(path);
  utf16_t wtitle(state.title);

  wchar_t *p = wfilters;
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
  ofn.hwndOwner = state.parent ? state.parent->p.hwnd : 0;
  ofn.lpstrFilter = wfilters;
  ofn.lpstrInitialDir = wpath;
  ofn.lpstrFile = wname;
  ofn.lpstrTitle = wtitle;
  ofn.nMaxFile = PATH_MAX;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"";

  bool result = (save == false ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn));
  if(result == false) return "";
  string name = (const char*)utf8_t(wname);
  name.transform("\\", "/");
  return name;
}

string pBrowserWindow::directory(BrowserWindow::State &state) {
  wchar_t wname[PATH_MAX + 1] = L"";

  BROWSEINFO bi;
  bi.hwndOwner = state.parent ? state.parent->p.hwnd : 0;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = wname;
  bi.lpszTitle = L"\nChoose a directory:";
  bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  bi.lpfn = BrowserWindowCallbackProc;
  bi.lParam = (LPARAM)&state;
  bi.iImage = 0;
  bool result = false;
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if(pidl) {
    if(SHGetPathFromIDList(pidl, wname)) {
      result = true;
      IMalloc *imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc))) {
        imalloc->Free(pidl);
        imalloc->Release();
      }
    }
  }
  if(result == false) return "";
  string name = (const char*)utf8_t(wname);
  if(!name) return "";
  name.transform("\\", "/");
  if(name.endswith("/") == false) name.append("/");
  return name;
}

string pBrowserWindow::open(BrowserWindow::State &state) {
  return BrowserWindow_fileDialog(0, state);
}

string pBrowserWindow::save(BrowserWindow::State &state) {
  return BrowserWindow_fileDialog(1, state);
}

}
