#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

struct stringref {
  operator const char*() const {
    return _data;
  }

  const char* data() const {
    return _data;
  }

  unsigned size() const {
    if(!_initialized) {
      _initialized = true;
      _size = strlen(_data);
    }
    return _size;
  }

  stringref() = delete;
  stringref(const stringref& source) = delete;
  stringref(stringref&& source) = delete;

  stringref(const char* source) {
    _data = source;
    _initialized = false;
  }

  stringref(const string& source) {
    _data = source.data();
    _size = source.size();
    _initialized = true;
  }

protected:
  const char* _data;
  mutable unsigned _size;
  mutable bool _initialized;
};

}

#endif
