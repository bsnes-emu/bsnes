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

  struct reverse_iterator {
    reverse_iterator(int position, int step = 0) : position(position), step(step) {}
    auto operator*() const -> int { return position; }
    auto operator!=(const reverse_iterator& source) const -> bool { return step > 0 ? position > source.position : position < source.position; }
    auto operator++() -> reverse_iterator& { position -= step; return *this; }

  private:
    int position;
    const int step;
  };

  auto begin() const -> iterator { return {origin, stride}; }
  auto end() const -> iterator { return {target}; }

  auto rbegin() const -> reverse_iterator { return {target - stride, stride}; }
  auto rend() const -> reverse_iterator { return {origin - stride}; }

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

}
