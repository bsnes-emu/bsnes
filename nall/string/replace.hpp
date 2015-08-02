#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive, bool Quoted>
auto _replace(string& self, rstring from, rstring to, long limit) -> string& {
  if(limit <= 0 || from.size() == 0) return self;

  signed size = self.size();
  signed matches = 0;
  signed quoted = 0;

  //count matches first, so that we only need to reallocate memory once
  //(recording matches would also require memory allocation, so this is not done)
  { const char* p = self.data();
    for(signed n = 0; n <= size - (signed)from.size();) {
      if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
      if(_compare<Insensitive>(p + n, size - n, from.data(), from.size())) { n++; continue; }

      if(++matches >= limit) break;
      n += from.size();
    }
  }
  if(matches == 0) return self;

  //in-place overwrite
  if(to.size() == from.size()) {
    char* p = self.get();

    for(signed n = 0, remaining = matches, quoted = 0; n <= size - (signed)from.size();) {
      if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
      if(_compare<Insensitive>(p + n, size - n, from.data(), from.size())) { n++; continue; }

      memory::copy(p + n, to.data(), to.size());

      if(!--remaining) break;
      n += from.size();
    }
  }

  //left-to-right shrink
  else if(to.size() < from.size()) {
    char* p = self.get();
    signed offset = 0;
    signed base = 0;

    for(signed n = 0, remaining = matches, quoted = 0; n <= size - (signed)from.size();) {
      if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n++; continue; } if(quoted) { n++; continue; } }
      if(_compare<Insensitive>(p + n, size - n, from.data(), from.size())) { n++; continue; }

      if(offset) memory::move(p + offset, p + base, n - base);
      memory::copy(p + offset + (n - base), to.data(), to.size());
      offset += (n - base) + to.size();

      n += from.size();
      base = n;
      if(!--remaining) break;
    }

    memory::move(p + offset, p + base, size - base);
    self.resize(size - matches * (from.size() - to.size()));
  }

  //right-to-left expand
  else if(to.size() > from.size()) {
    self.resize(size + matches * (to.size() - from.size()));
    char* p = self.get();

    signed offset = self.size();
    signed base = size;

    for(signed n = size, remaining = matches; n >= (signed)from.size();) {  //quoted reused from parent scope since we are iterating backward
      if(Quoted) { if(p[n] == '\"') { quoted ^= 1; n--; continue; } if(quoted) { n--; continue; } }
      if(_compare<Insensitive>(p + n - from.size(), size - n + from.size(), from.data(), from.size())) { n--; continue; }

      memory::move(p + offset - (base - n), p + base - (base - n), base - n);
      memory::copy(p + offset - (base - n) - to.size(), to.data(), to.size());
      offset -= (base - n) + to.size();

      if(!--remaining) break;
      n -= from.size();
      base = n;
    }
  }

  return self;
}

auto replace(string& self, rstring from, rstring to, long limit) -> string& { return _replace<0, 0>(self, from, to, limit); }
auto ireplace(string& self, rstring from, rstring to, long limit) -> string& { return _replace<1, 0>(self, from, to, limit); }
auto qreplace(string& self, rstring from, rstring to, long limit) -> string& { return _replace<0, 1>(self, from, to, limit); }
auto iqreplace(string& self, rstring from, rstring to, long limit) -> string& { return _replace<1, 1>(self, from, to, limit); }

};

#endif
