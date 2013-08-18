#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<unsigned Limit, bool Insensitive, bool Quoted>
string& string::ureplace(rstring key, rstring token) {
  if(key.size() == 0) return *this;
  enum : unsigned { limit = Limit ? Limit : ~0u };

  const char* p = data();
  unsigned counter = 0;

  while(*p) {
    if(quoteskip<Quoted>(p)) continue;
    for(unsigned n = 0;; n++) {
      if(key[n] == 0) { counter++; p += n; break; }
      if(!chrequal<Insensitive>(key[n], p[n])) { p++; break; }
    }
  }
  if(counter == 0) return *this;
  if(Limit) counter = min(counter, Limit);

  char* t = data();
  char* base = nullptr;
  signed displacement = token.size() - key.size();
  signed displacementSize = displacement * counter;

  if(token.size() > key.size()) {
    t = base = strdup(data());
    reserve((unsigned)(p - data()) + displacementSize);
  }
  char* o = data();

  while(*t && counter) {
    if(quotecopy<Quoted>(o, t)) continue;
    for(unsigned n = 0;; n++) {
      if(key[n] == 0) { counter--; memcpy(o, token, token.size()); t += key.size(); o += token.size(); break; }
      if(!chrequal<Insensitive>(key[n], t[n])) { *o++ = *t++; break; }
    }
  }
  do *o++ = *t; while(*t++);
  if(base) free(base);

  resize(_size + displacementSize);
  return *this;
}

template<unsigned Limit> string& string::replace(rstring key, rstring token) { return ureplace<Limit, false, false>(key, token); }
template<unsigned Limit> string& string::ireplace(rstring key, rstring token) { return ureplace<Limit, true, false>(key, token); }
template<unsigned Limit> string& string::qreplace(rstring key, rstring token) { return ureplace<Limit, false, true>(key, token); }
template<unsigned Limit> string& string::iqreplace(rstring key, rstring token) { return ureplace<Limit, true, true>(key, token); }

};

#endif
