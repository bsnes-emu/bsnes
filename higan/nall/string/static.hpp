#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string string::read(const string& filename) {
  string data;
  data.readfile(filename);
  return data;
}

}

#endif
