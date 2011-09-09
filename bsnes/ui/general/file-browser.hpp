struct FileBrowser : Window {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
  LineEdit pathEdit;
  Button pathBrowse;
  Button pathUp;
  ListView fileList;
  lstring fileNameList;

  void open(const string &title, const lstring &filterList, function<void (string)> callback);

  FileBrowser();

private:
  string activePath;
  lstring filterList;
  function<void (string)> callback;

  void setPath(const string &path);
  void fileListActivate();
};

extern FileBrowser *fileBrowser;
