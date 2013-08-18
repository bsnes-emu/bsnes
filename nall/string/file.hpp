#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string string::read(const string& filename) {
  string result;

  #if !defined(_WIN32)
  FILE* fp = fopen(filename, "rb");
  #else
  FILE* fp = _wfopen(utf16_t(filename), L"rb");
  #endif
  if(!fp) return result;

  fseek(fp, 0, SEEK_END);
  unsigned fsize = ftell(fp);
  rewind(fp);
  char* fdata = new char[fsize + 1];
  unsigned unused = fread(fdata, 1, fsize, fp);
  fclose(fp);
  fdata[fsize] = 0;
  result.resize(fsize);
  memcpy(result.data(), fdata, fsize);
  delete[] fdata;

  return result;
}

}

#endif
