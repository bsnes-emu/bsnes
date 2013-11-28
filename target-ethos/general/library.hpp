struct LibraryBrowser : VerticalLayout {
  ListView folders;
  HorizontalLayout informationLayout;
    Label informationType;
    Label information;

  LibraryBrowser();
  void onActivate();
  void refresh();
  void setFilter(const string& filter);
  void setInformation();
  void setPath(const string& pathname);

  string filter;
  string filterMask;
  string filterSuffix;
  string pathname;
};

struct LibraryManager : Window {
  VerticalLayout layout;
  TabFrame libraryFrame;
    VerticalLayout importLayout;
      Label importInformation;
      Button importButton;
    vector<LibraryBrowser*> browsers;

  LibraryManager();
  void bootstrap();
  string load(const string& type);
  void setVisible(bool visible = true);

  bool slotLoad = false;
  string loadPathname;
};

extern LibraryManager* libraryManager;
