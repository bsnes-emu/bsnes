#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

optional<unsigned> lstring::find(rstring key) const {
  for(unsigned i = 0; i < size(); i++) {
    if(operator[](i) == key) return {true, i};
  }
  return false;
}

string lstring::merge(const string& separator) const {
  string output;
  for(unsigned i = 0; i < size(); i++) {
    output.append(operator[](i));
    if(i < size() - 1) output.append(separator);
  }
  return output;
}

lstring& lstring::isort() {
  nall::sort(pool, objectsize, [](const string& x, const string& y) {
    return istrcmp(x, y) < 0;
  });
  return *this;
}

lstring& lstring::strip() {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).strip();
  }
  return *this;
}

template<typename... Args> void lstring::append(const string& data, Args&&... args) {
  vector::append(data);
  append(std::forward<Args>(args)...);
}

bool lstring::operator==(const lstring& source) const {
  if(this == &source) return true;
  if(size() != source.size()) return false;
  for(unsigned n = 0; n < size(); n++) {
    if(operator[](n) != source[n]) return false;
  }
  return true;
}

bool lstring::operator!=(const lstring& source) const {
  return !operator==(source);
}

lstring& lstring::operator=(const lstring& source) {
  vector::operator=(source);
  return *this;
}

lstring& lstring::operator=(lstring& source) {
  vector::operator=(source);
  return *this;
}

lstring& lstring::operator=(lstring&& source) {
  vector::operator=(std::move(source));
  return *this;
}

template<typename... Args> lstring::lstring(Args&&... args) {
  append(std::forward<Args>(args)...);
}

lstring::lstring(const lstring& source) {
  vector::operator=(source);
}

lstring::lstring(lstring& source) {
  vector::operator=(source);
}

lstring::lstring(lstring&& source) {
  vector::operator=(std::move(source));
}

}

#endif
