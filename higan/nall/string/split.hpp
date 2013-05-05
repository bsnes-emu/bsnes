#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<unsigned Limit, bool Insensitive, bool Quoted> lstring& lstring::usplit(rstring key, rstring base) {
  reset();
  if(key.size() == 0) return *this;

  const char* b = base;
  const char* p = base;

  while(*p) {
    if(Limit) if(size() >= Limit) break;
    if(quoteskip<Quoted>(p)) continue;
    for(unsigned n = 0;; n++) {
      if(key[n] == 0) {
        append(substr(b, 0, p - b));
        p += n;
        b = p;
        break;
      }
      if(!chrequal<Insensitive>(key[n], p[n])) { p++; break; }
    }
  }

  append(b);
  return *this;
}

template<unsigned Limit> lstring& lstring::split(rstring key, rstring src) { return usplit<Limit, false, false>(key, src); }
template<unsigned Limit> lstring& lstring::isplit(rstring key, rstring src) { return usplit<Limit, true, false>(key, src); }
template<unsigned Limit> lstring& lstring::qsplit(rstring key, rstring src) { return usplit<Limit, false, true>(key, src); }
template<unsigned Limit> lstring& lstring::iqsplit(rstring key, rstring src) { return usplit<Limit, true, true>(key, src); }

};

#endif
