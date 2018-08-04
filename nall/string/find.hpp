#pragma once

namespace nall {

auto string::contains(view<string> characters) const -> maybe<uint> {
  for(uint x : range(size())) {
    for(char y : characters) {
      if(operator[](x) == y) return x;
    }
  }
  return nothing;
}

template<bool Insensitive, bool Quoted> auto string::_find(int offset, view<string> source) const -> maybe<uint> {
  if(source.size() == 0) return nothing;

  auto p = data();
  for(uint n = offset, quoted = 0; n < size();) {
    if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
    if(_compare<Insensitive>(p + n, size() - n, source.data(), source.size())) { n++; continue; }
    return n - offset;
  }

  return nothing;
}

auto string::find(view<string> source) const -> maybe<uint> { return _find<0, 0>(0, source); }
auto string::ifind(view<string> source) const -> maybe<uint> { return _find<1, 0>(0, source); }
auto string::qfind(view<string> source) const -> maybe<uint> { return _find<0, 1>(0, source); }
auto string::iqfind(view<string> source) const -> maybe<uint> { return _find<1, 1>(0, source); }

auto string::findFrom(int offset, view<string> source) const -> maybe<uint> { return _find<0, 0>(offset, source); }
auto string::ifindFrom(int offset, view<string> source) const -> maybe<uint> { return _find<1, 0>(offset, source); }

}
