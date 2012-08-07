struct CShellExt : IContextMenu, IShellExtInit {
protected:
  unsigned instanceCount;
  lstring fileList;

public:
  CShellExt();
  ~CShellExt();

  STDMETHODIMP QueryInterface(REFIID, LPVOID*);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
  STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
  STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT FAR*, LPSTR, UINT);
  STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

private:
  vector<unsigned> matchedRules();
};

typedef CShellExt *LPCSHELLEXT;