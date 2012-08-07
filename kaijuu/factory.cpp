CShellExtClassFactory::CShellExtClassFactory() {
  instanceCount = 0;
  referenceCount++;
}

CShellExtClassFactory::~CShellExtClassFactory() {
  referenceCount--;
}

STDMETHODIMP CShellExtClassFactory::QueryInterface(REFIID riid, LPVOID *ppv) {
  *ppv = NULL;
  if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory)) {
    *ppv = (LPCLASSFACTORY)this;
    AddRef();
    return NOERROR;
  }
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CShellExtClassFactory::AddRef() {
  return ++instanceCount;
}

STDMETHODIMP_(ULONG) CShellExtClassFactory::Release() {
  if(--instanceCount) return instanceCount;
  delete this;
  return 0;
}

STDMETHODIMP CShellExtClassFactory::CreateInstance(LPUNKNOWN pUnknownOuter, REFIID riid, LPVOID *ppv) {
  *ppv = NULL;
  if(pUnknownOuter) return CLASS_E_NOAGGREGATION;
  CShellExt *pShellExt = new CShellExt();
  if(pShellExt == NULL) return E_OUTOFMEMORY;
  return pShellExt->QueryInterface(riid, ppv);
}

STDMETHODIMP CShellExtClassFactory::LockServer(BOOL fLock) {
  return NOERROR;
}
