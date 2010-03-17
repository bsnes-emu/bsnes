class Reader : public library {
public:
  string compressionList;
  string extensionList;

  function<const char* ()> supported;
  function<bool (string&, uint8_t*&, unsigned&)> load;
  function<bool (string&, const uint8_t*, unsigned)> map;

  const char* direct_supported();
  bool direct_load(string&, uint8_t*&, unsigned&);
  bool direct_map(string&, const uint8_t*, unsigned);

  Reader();
};

extern Reader reader;
