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
      _size = strlen(_data);
      _initialized = 1;
    }
    return _size;
  }

  stringref() = delete;
  stringref(const stringref& source) = delete;
  stringref(stringref&& source) = delete;

  template<typename T, typename... Args>
  stringref(T&& source, Args&&... args) {
    if(sizeof...(Args) == 0) { construct(std::forward<T>(source)); _string = nullptr; return; }
    _string = new string(std::forward<T>(source), std::forward<Args>(args)...);
    _data = _string->data();
    _size = _string->size();
    _initialized = 1;
  }

  ~stringref() {
    if(_string) delete _string;
  }

protected:
  const char* _data;
  string* _string;
  mutable unsigned _size;
  mutable bool _initialized;

  template<int size>
  void construct(const char (&source)[size]) {
    _data = source;
    _size = size - 1;
    _initialized = 1;
  }

  template<typename T, typename = typename std::enable_if<std::is_same<T, char>::value>::type>
  void construct(const T* source) {
    _data = source;
    _size = 0;
    _initialized = 0;
  }

  template<typename T, typename = typename std::enable_if<std::is_same<T, string>::value>::type>
  void construct(const T& source) {
    _data = source.data();
    _size = source.size();
    _initialized = 1;
  }
};

}

#endif
