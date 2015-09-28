#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive, bool Quoted>
auto lstring::_split(rstring source, rstring find, long limit) -> lstring& {
  reset();
  if(limit <= 0 || find.size() == 0) return *this;

  const char* p = source.data();
  signed size = source.size();
  signed base = 0;
  signed matches = 0;

  for(signed n = 0, quoted = 0; n <= size - (signed)find.size();) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(string::_compare<Insensitive>(p + n, size - n, find.data(), find.size())) { n++; continue; }
    if(matches >= limit) break;

    string& s = operator()(matches);
    s.resize(n - base);
    memory::copy(s.get(), p + base, n - base);

    n += find.size();
    base = n;
    matches++;
  }

  string& s = operator()(matches);
  s.resize(size - base);
  memory::copy(s.get(), p + base, size - base);

  return *this;
}

auto string::split(rstring on, long limit) const -> lstring { return lstring()._split<0, 0>(*this, on, limit); }
auto string::isplit(rstring on, long limit) const -> lstring { return lstring()._split<1, 0>(*this, on, limit); }
auto string::qsplit(rstring on, long limit) const -> lstring { return lstring()._split<0, 1>(*this, on, limit); }
auto string::iqsplit(rstring on, long limit) const -> lstring { return lstring()._split<1, 1>(*this, on, limit); }

}

#endif
