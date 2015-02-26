#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

struct stringref {
  operator const char*() const {
    return _data;
  }

  auto data() const -> const char* {
    return _data;
  }

  auto size() const -> unsigned {
    if(!_initialized) _size = strlen(_data), _initialized = true;
    return _size;
  }

  stringref() {
    _string = nullptr;
    _data = "";
    _size = 0;
    _initialized = true;
  }

  stringref(const char* source) {
    _string = nullptr;
    _data = source;
    _initialized = false;
  }

  stringref(const string& source) {
    _string = nullptr;
    _data = source.data();
    _size = source.size();
    _initialized = true;
  }

  template<typename... P> stringref(P&&... p) {
    _string = new string{std::forward<P>(p)...};
    _data = _string->data();
    _size = _string->size();
    _initialized = true;
  }

  ~stringref() {
    if(_string) delete _string;
  }

  stringref(const stringref& source) = delete;
  stringref(stringref&& source) = delete;

protected:
  string* _string;
  const char* _data;
  mutable signed _size;
  mutable unsigned _initialized;
};

}

#endif
