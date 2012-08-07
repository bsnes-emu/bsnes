#include "kaijuu.hpp"
#include "extension.cpp"
#include "factory.cpp"

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstance, DWORD dwReason, LPVOID lpReserved) {
  if(dwReason == DLL_PROCESS_ATTACH) {
    module = hinstance;
  }

  if(dwReason == DLL_PROCESS_DETACH) {
  }

  return TRUE;
}

STDAPI DllCanUnloadNow() {
  return referenceCount == 0 ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
  wchar_t fileName[MAX_PATH];
  GetModuleFileNameW(module, fileName, MAX_PATH);
  registry::write({"HKCR/CLSID/", classID, "/"}, classDescription);
  registry::write({"HKCR/CLSID/", classID, "/shellex/MayChangeDefaultMenu/"});
  registry::write({"HKCR/CLSID/", classID, "/InprocServer32/"}, (const char*)utf8_t(fileName));
  registry::write({"HKCR/CLSID/", classID, "/InprocServer32/ThreadingModel"}, "Apartment");
  registry::write({"HKCR/*/shellex/ContextMenuHandlers/", classDescription, "/"}, classID);
  registry::write({"HKCR/Directory/shellex/ContextMenuHandlers/", classDescription, "/"}, classID);
  registry::write({"HKLM/Software/Microsoft/Windows/CurrentVersion/Shell Extensions/Approved/", classID}, classDescription);
  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  return S_OK;
}

STDAPI DllUnregisterServer() {
  registry::remove({"HKCR/CLSID/", classID, "/"});
  registry::remove({"HKCR/*/shellex/ContextMenuHandlers/", classDescription, "/"});
  registry::remove({"HKCR/Directory/shellex/ContextMenuHandlers/", classDescription, "/"});
  registry::remove({"HKLM/Software/Microsoft/Windows/CurrentVersion/Shell Extensions/Approved/", classID});
  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  return S_OK;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv) {
  *ppv = NULL;
  if(IsEqualIID(rclsid, CLSID_ShellExtension)) {
    CShellExtClassFactory *pcf = new CShellExtClassFactory;
    return pcf->QueryInterface(riid, ppv);
  }
  return CLASS_E_CLASSNOTAVAILABLE;
}
