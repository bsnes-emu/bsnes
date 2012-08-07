#ifdef _WIN64
  DEFINE_GUID(CLSID_ShellExtension, 0x5c8ee773, 0xb9dc, 0xcec7, 0x63, 0x31, 0x98, 0x6c, 0x16, 0x2b, 0x15, 0x0a);
  const char classID[] = "{5c8ee773-b9dc-cec7-6331-986c162b150a}";
  const char classDescription[] = "KaijuuOpenShEx64";
  const char classDriver[] = "kaijuu64.dll";
#else
  DEFINE_GUID(CLSID_ShellExtension, 0xa2f1781c, 0x2e37, 0x9bcd, 0x66, 0x93, 0xc9, 0xe4, 0x52, 0x89, 0xc4, 0xc2);
  const char classID[] = "{a2f1781c-2e37-9bcd-6693-c9e45289c4c2}";
  const char classDescription[] = "KaijuuOpenShEx32";
  const char classDriver[] = "kaijuu32.dll";
#endif
