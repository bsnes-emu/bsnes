#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive, bool Quoted> auto _find(const string& self, signed offset, rstring source) -> maybe<unsigned> {
  if(source.size() == 0) return nothing;

  const char* p = self.data();
  unsigned size = self.size();

  for(unsigned n = offset, quoted = 0; n < size;) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(_compare<Insensitive>(p + n, size - n, source.data(), source.size())) { n++; continue; }
    return n - offset;
  }

  return nothing;
}

auto find(const string& self, rstring source) -> maybe<unsigned> { return _find<0, 0>(self, 0, source); }
auto ifind(const string& self, rstring source) -> maybe<unsigned> { return _find<1, 0>(self, 0, source); }
auto qfind(const string& self, rstring source) -> maybe<unsigned> { return _find<0, 1>(self, 0, source); }
auto iqfind(const string& self, rstring source) -> maybe<unsigned> { return _find<1, 1>(self, 0, source); }

auto findFrom(const string& self, signed offset, rstring source) -> maybe<unsigned> { return _find<0, 0>(self, offset, source); }
auto ifindFrom(const string& self, signed offset, rstring source) -> maybe<unsigned> { return _find<1, 0>(self, offset, source); }

}

#endif
