#ifdef NALL_STRING_INTERNAL_HPP

//usage example:
//if(auto position = strpos(str, key)) print(position(), "\n");
//prints position of key within str; but only if it is found

namespace nall {

template<bool Insensitive, bool Quoted>
maybe<unsigned> ustrpos(const char* str, const char* key) {
  const char* base = str;

  while(*str) {
    if(quoteskip<Quoted>(str)) continue;
    for(unsigned n = 0;; n++) {
      if(key[n] == 0) return (unsigned)(str - base);
      if(str[n] == 0) return nothing;
      if(!chrequal<Insensitive>(str[n], key[n])) break;
    }
    str++;
  }

  return nothing;
}

maybe<unsigned> strpos(const char* str, const char* key) { return ustrpos<false, false>(str, key); }
maybe<unsigned> istrpos(const char* str, const char* key) { return ustrpos<true, false>(str, key); }
maybe<unsigned> qstrpos(const char* str, const char* key) { return ustrpos<false, true>(str, key); }
maybe<unsigned> iqstrpos(const char* str, const char* key) { return ustrpos<true, true>(str, key); }

}

#endif
