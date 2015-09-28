#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive, bool Quoted> auto string::_find(signed offset, rstring source) const -> maybe<unsigned> {
  if(source.size() == 0) return nothing;

  auto p = data();
  for(unsigned n = offset, quoted = 0; n < size();) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(_compare<Insensitive>(p + n, size() - n, source.data(), source.size())) { n++; continue; }
    return n - offset;
  }

  return nothing;
}

auto string::find(rstring source) const -> maybe<unsigned> { return _find<0, 0>(0, source); }
auto string::ifind(rstring source) const -> maybe<unsigned> { return _find<1, 0>(0, source); }
auto string::qfind(rstring source) const -> maybe<unsigned> { return _find<0, 1>(0, source); }
auto string::iqfind(rstring source) const -> maybe<unsigned> { return _find<1, 1>(0, source); }

auto string::findFrom(signed offset, rstring source) const -> maybe<unsigned> { return _find<0, 0>(offset, source); }
auto string::ifindFrom(signed offset, rstring source) const -> maybe<unsigned> { return _find<1, 0>(offset, source); }

}

#endif
