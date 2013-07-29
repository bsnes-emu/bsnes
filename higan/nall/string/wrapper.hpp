#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<unsigned limit> lstring string::split(rstring key) const { lstring result; result.split<limit>(key, data()); return result; }
template<unsigned limit> lstring string::isplit(rstring key) const { lstring result; result.isplit<limit>(key, data()); return result; }
template<unsigned limit> lstring string::qsplit(rstring key) const { lstring result; result.qsplit<limit>(key, data()); return result; }
template<unsigned limit> lstring string::iqsplit(rstring key) const { lstring result; result.iqsplit<limit>(key, data()); return result; }

bool string::match(rstring source) const { return nall::strmatch(data(), source); }
bool string::imatch(rstring source) const { return nall::istrmatch(data(), source); }

signed string::compare(rstring source) const {
  return strcmp(data(), source.data());
}

signed string::icompare(rstring source) const {
  return istrcmp(data(), source.data());
}

bool string::equals(rstring source) const {
  if(size() != source.size()) return false;
  return compare(source) == 0;
}

bool string::iequals(rstring source) const {
  if(size() != source.size()) return false;
  return icompare(source) == 0;
}

bool string::beginswith(rstring source) const {
  if(source.size() > size()) return false;
  return memcmp(data(), source.data(), source.size()) == 0;
}

bool string::ibeginswith(rstring source) const {
  if(source.size() > size()) return false;
  return imemcmp(data(), source.data(), source.size()) == 0;
}

bool string::endswith(rstring source) const {
  if(source.size() > size()) return false;
  return memcmp(data() + size() - source.size(), source.data(), source.size()) == 0;
}

bool string::iendswith(rstring source) const {
  if(source.size() > size()) return false;
  return imemcmp(data() + size() - source.size(), source.data(), source.size()) == 0;
}

string string::slice(unsigned offset, unsigned length) const {
  if(offset >= size()) return "";
  if(length == ~0u) length = size() - offset;
  return substr(data(), offset, length);
}

string& string::lower() { nall::strlower(data()); return *this; }
string& string::upper() { nall::strupper(data()); return *this; }
string& string::qlower() { nall::qstrlower(data()); return *this; }
string& string::qupper() { nall::qstrupper(data()); return *this; }

string& string::transform(rstring before, rstring after) { nall::strtr(data(), before, after); return *this; }

string& string::reverse() {
  unsigned length = size(), pivot = length >> 1;
  for(signed x = 0, y = length - 1; x < pivot && y >= 0; x++, y--) std::swap(data()[x], data()[y]);
  return *this;
}

template<unsigned Limit> string& string::ltrim(rstring key) {
  if(key.size() == 0) return *this;
  unsigned limit = Limit ? Limit : ~0u, offset = 0;

  while(limit && size() - offset >= key.size()) {
    if(memcmp(data() + offset, key.data(), key.size())) break;
    offset += key.size();
    limit--;
  }

  if(offset) memmove(data(), data() + offset, size() - offset);
  resize(size() - offset);
  return *this;
}

template<unsigned Limit> string& string::rtrim(rstring key) {
  if(key.size() == 0) return *this;
  unsigned limit = Limit ? Limit : ~0u, offset = 0;

  while(limit && size() - offset >= key.size()) {
    if(memcmp(data() + size() - key.size() - offset, key.data(), key.size())) break;
    offset += key.size();
    limit--;
  }

  resize(size() - offset);
  return *this;
}

template<unsigned Limit> string& string::trim(rstring key) {
  rtrim<Limit>(key);
  ltrim<Limit>(key);
  return *this;
}

template<unsigned Limit> string& string::trim(rstring lkey, rstring rkey) {
  rtrim<Limit>(rkey);
  ltrim<Limit>(lkey);
  return *this;
}

string& string::strip() {
  nall::strip(data());
  resize(length());
  return *this;
}

optional<unsigned> string::find(rstring key) const { return strpos(data(), key); }
optional<unsigned> string::ifind(rstring key) const { return istrpos(data(), key); }
optional<unsigned> string::qfind(rstring key) const { return qstrpos(data(), key); }
optional<unsigned> string::iqfind(rstring key) const { return iqstrpos(data(), key); }

}

#endif
