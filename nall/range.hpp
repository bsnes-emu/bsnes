#pragma once

namespace nall {

struct range_t {
  struct iterator {
    iterator(int position, int step = 0) : position(position), step(step) {}
    auto operator*() const -> int { return position; }
    auto operator!=(const iterator& source) const -> bool { return step > 0 ? position < source.position : position > source.position; }
    auto operator++() -> iterator& { position += step; return *this; }

  private:
    int position;
    const int step;
  };

  auto begin() const -> const iterator { return iterator(origin, stride); }
  auto end() const -> const iterator { return iterator(target); }

  int origin;
  int target;
  int stride;
};

inline auto range(int size) {
  return range_t{0, size, 1};
}

inline auto range(int offset, int size) {
  return range_t{offset, size, 1};
}

inline auto range(int offset, int size, int step) {
  return range_t{offset, size, step};
}

//reverse-range
inline auto rrange(int size) {
  return range_t{size - 1, -1, -1};
}

}
