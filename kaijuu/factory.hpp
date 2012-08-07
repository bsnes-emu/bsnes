struct CShellExtClassFactory : IClassFactory {
protected:
  unsigned instanceCount;

public:
  CShellExtClassFactory();
  ~CShellExtClassFactory();

  STDMETHODIMP QueryInterface(REFIID, LPVOID FAR*);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  STDMETHODIMP CreateInstance(LPUNKNOWN, REFIID, LPVOID FAR*);
  STDMETHODIMP LockServer(BOOL);
};

typedef CShellExtClassFactory *LPCSHELLEXTCLASSFACTORY;
