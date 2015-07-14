#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive, bool Quoted> auto _split(lstring& self, rstring source, rstring find, long limit) -> lstring& {
  self.reset();
  if(limit <= 0 || find.size() == 0) return self;

  const char* p = source.data();
  signed size = source.size();
  signed base = 0;
  signed matches = 0;

  for(signed n = 0, quoted = 0; n <= size - (signed)find.size();) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(_compare<Insensitive>(p + n, size - n, find.data(), find.size())) { n++; continue; }
    if(matches >= limit) break;

    string& s = self(matches);
    s.resize(n - base);
    memory::copy(s.pointer(), p + base, n - base);

    n += find.size();
    base = n;
    matches++;
  }

  string& s = self(matches);
  s.resize(size - base);
  memory::copy(s.pointer(), p + base, size - base);

  return self;
}

auto split(string& self, rstring on, long limit) -> lstring { return lstring().split(self, on, limit); }
auto isplit(string& self, rstring on, long limit) -> lstring { return lstring().isplit(self, on, limit); }
auto qsplit(string& self, rstring on, long limit) -> lstring { return lstring().qsplit(self, on, limit); }
auto iqsplit(string& self, rstring on, long limit) -> lstring { return lstring().iqsplit(self, on, limit); }

auto string::split(rstring on, long limit) const -> lstring { return lstring().split(*this, on, limit); }
auto string::isplit(rstring on, long limit) const -> lstring { return lstring().isplit(*this, on, limit); }
auto string::qsplit(rstring on, long limit) const -> lstring { return lstring().qsplit(*this, on, limit); }
auto string::iqsplit(rstring on, long limit) const -> lstring { return lstring().iqsplit(*this, on, limit); }

}

#endif
