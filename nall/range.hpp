#ifndef NALL_RANGE_HPP
#define NALL_RANGE_HPP

namespace nall {

struct range_t {
  struct iterator {
    iterator(signed position, signed step = 0) : position(position), step(step) {}
    auto operator*() const -> signed { return position; }
    auto operator!=(const iterator& source) const -> bool { return step > 0 ? position < source.position : position > source.position; }
    auto operator++() -> iterator& { position += step; return *this; }

  private:
    signed position;
    const signed step;
  };

  auto begin() const -> const iterator { return iterator(origin, stride); }
  auto end() const -> const iterator { return iterator(target); }

  signed origin;
  signed target;
  signed stride;
};

inline auto range(signed size) {
  return range_t{0, size, 1};
}

inline auto range(signed offset, signed size) {
  return range_t{offset, size, 1};
}

inline auto range(signed offset, signed size, signed step) {
  return range_t{offset, size, step};
}

inline auto rangeReverse(signed size) {
  return range_t{size - 1, -1, -1};
}

template<typename T> inline auto range(const vector<T>& container) {
  return range_t{0, (signed)container.size(), 1};
}

}

#endif
