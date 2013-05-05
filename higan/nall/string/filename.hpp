#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

// "/foo/bar.c" -> "/foo/"
// "/foo/" -> "/foo/"
// "bar.c" -> "./"
string dir(string name) {
  for(signed i = name.length(); i >= 0; i--) {
    if(name[i] == '/' || name[i] == '\\') {
      name[i + 1] = 0;
      break;
    }
    if(i == 0) name = "./";
  }
  return name;
}

// "/foo/bar.c" -> "bar.c"
// "/foo/" -> ""
// "bar.c" -> "bar.c"
string notdir(string name) {
  for(signed i = name.length(); i >= 0; i--) {
    if(name[i] == '/' || name[i] == '\\') {
      return (const char*)name + i + 1;
    }
  }
  return name;
}

// "/foo/bar/baz" -> "/foo/bar/"
// "/foo/bar/" -> "/foo/"
// "/foo/bar" -> "/foo/"
string parentdir(string name) {
  unsigned length = name.length(), paths = 0, prev, last;
  for(unsigned i = 0; i < length; i++) {
    if(name[i] == '/' || name[i] == '\\') {
      paths++;
      prev = last;
      last = i;
    }
  }
  if(last + 1 == length) last = prev;  //if name ends in slash; use previous slash
  if(paths > 1) name[last + 1] = 0;
  return name;
}

// "/foo/bar.c" -> "/foo/bar"
string basename(string name) {
  for(signed i = name.length(); i >= 0; i--) {
    if(name[i] == '/' || name[i] == '\\') break;  //file has no extension
    if(name[i] == '.') {
      name[i] = 0;
      break;
    }
  }
  return name;
}

// "/foo/bar.c" -> "c"
// "/foo/bar" -> ""
string extension(string name) {
  for(signed i = name.length(); i >= 0; i--) {
    if(name[i] == '/' || name[i] == '\\') return "";  //file has no extension
    if(name[i] == '.') {
      return (const char*)name + i + 1;
    }
  }
  return name;
}

}

#endif
