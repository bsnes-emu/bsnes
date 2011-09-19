struct FileBrowser : Window {
  VerticalLayout layout;
  HorizontalLayout pathLayout;
    LineEdit pathEdit;
    Button pathBrowse;
    Button pathUp;
  ListView fileList;
  HorizontalLayout controlLayout;
  Label filterLabel;
  Button openButton;

  void open(const string &title, const lstring &filterList, function<void (string)> callback);

  FileBrowser();

private:
  string activePath;
  lstring filterList;
  lstring fileNameList;
  function<void (string)> callback;

  void setPath(const string &path);
  void fileListActivate();
};

extern FileBrowser *fileBrowser;
