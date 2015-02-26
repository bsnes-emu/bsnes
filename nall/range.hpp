#ifndef NALL_RANGE_HPP
#define NALL_RANGE_HPP

namespace nall {

struct range_t {
  struct iterator {
    signed operator*() const { return position; }
    bool operator!=(const iterator& source) const { return step > 0 ? position < source.position : position > source.position; }
    iterator& operator++() { position += step; return *this; }
    iterator(signed position, signed step = 0) : position(position), step(step) {}

  private:
    signed position;
    const signed step;
  };

  const iterator begin() const { return iterator(origin, stride); }
  const iterator end() const { return iterator(target); }

  signed origin;
  signed target;
  signed stride;
};

inline range_t range(signed size) {
  return range_t{0, size, 1};
}

inline range_t range(signed offset, signed size) {
  return range_t{offset, size, 1};
}

inline range_t range(signed offset, signed size, signed step) {
  return range_t{offset, size, step};
}

inline range_t range_reverse(signed size) {
  return range_t{size - 1, -1, -1};
}

template<typename T> inline range_t range(const vector<T>& container) {
  return range_t{0, (signed)container.size(), 1};
}

}

#endif
