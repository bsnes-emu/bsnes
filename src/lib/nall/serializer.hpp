#ifndef NALL_SERIALIZER_HPP
#define NALL_SERIALIZER_HPP

#include <nall/new.hpp>
#include <nall/stdint.hpp>
#include <nall/traits.hpp>
#include <nall/utility.hpp>

namespace nall {

  //serializer: a class designed to save and restore the state of classes.
  //
  //benefits:
  //- data() will be portable in size (it is not necessary to specify type sizes.)
  //- data() will be portable in endianness (always stored internally as little-endian.)
  //- one serialize function can both save and restore class states.
  //
  //caveats:
  //- only plain-old-data can be stored. complex classes must provide serialize(serializer&);
  //- floating-point values are not supported.

  class serializer {
  public:
    const uint8_t* data() const {
      return idata;
    }

    unsigned size() const {
      return isize;
    }

    unsigned capacity() const {
      return icapacity;
    }

    template<typename T> void integer(T &value) {
      if(save) {
        for(unsigned n = 0; n < sizeof(T); n++) idata[isize++] = value >> (n << 3);
      } else {
        value = 0;
        for(unsigned n = 0; n < sizeof(T); n++) value |= idata[isize++] << (n << 3);
      }
    }

    template<typename T> void array(T &array) {
      enum { size = sizeof(T) / sizeof(typename remove_extent<T>::type) };
      for(unsigned n = 0; n < size; n++) integer(array[n]);
    }

    template<typename T> void array(T array, unsigned size) {
      for(unsigned n = 0; n < size; n++) integer(array[n]);
    }

    serializer& operator=(const serializer &s) {
      if(idata) delete[] idata;

      save = s.save;
      idata = new uint8_t[s.icapacity];
      isize = s.isize;
      icapacity = s.icapacity;

      memcpy(idata, s.idata, s.icapacity);
      return *this;
    }

    serializer(const serializer &s) : idata(0) {
      operator=(s);
    }

    serializer(unsigned capacity) {
      save = true;
      idata = new(zeromemory) uint8_t[capacity];
      isize = 0;
      icapacity = capacity;
    }

    serializer(const uint8_t *data, unsigned capacity) {
      save = false;
      idata = new uint8_t[capacity];
      isize = 0;
      icapacity = capacity;
      memcpy(idata, data, capacity);
    }

    ~serializer() {
      delete[] idata;
    }

  private:
    bool save;
    uint8_t *idata;
    unsigned isize;
    unsigned icapacity;
  };

};

#endif
