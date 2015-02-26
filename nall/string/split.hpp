#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<unsigned Limit, bool Insensitive, bool Quoted> auto _split(lstring& self, rstring source, rstring find) -> lstring& {
  self.reset();
  if(find.size() == 0) return self;

  const char* p = source.data();
  signed size = source.size();
  signed base = 0;
  signed matches = 0;

  for(signed n = 0, quoted = 0; n <= size - (signed)find.size();) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(_compare<Insensitive>(p + n, size - n, find.data(), find.size())) { n++; continue; }
    if(matches >= Limit) break;

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

template<unsigned L> auto split(string& self, rstring on) -> lstring { return lstring().split<L>(self, on); }
template<unsigned L> auto isplit(string& self, rstring on) -> lstring { return lstring().isplit<L>(self, on); }
template<unsigned L> auto qsplit(string& self, rstring on) -> lstring { return lstring().qsplit<L>(self, on); }
template<unsigned L> auto iqsplit(string& self, rstring on) -> lstring { return lstring().iqsplit<L>(self, on); }

template<unsigned L> auto string::split(rstring on) const -> lstring { return lstring().split<L>(*this, on); }
template<unsigned L> auto string::isplit(rstring on) const -> lstring { return lstring().isplit<L>(*this, on); }
template<unsigned L> auto string::qsplit(rstring on) const -> lstring { return lstring().qsplit<L>(*this, on); }
template<unsigned L> auto string::iqsplit(rstring on) const -> lstring { return lstring().iqsplit<L>(*this, on); }

}

#endif
