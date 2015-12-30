template<typename T, unsigned size>
struct ModuloArray {
  inline auto operator[](signed index) const -> T {
    return buffer[size + index];
  }

  inline auto read(signed index) const -> T {
    return buffer[size + index];
  }

  inline auto write(unsigned index, const T value) -> void {
    buffer[index] =
    buffer[index + size] =
    buffer[index + size + size] = value;
  }

  auto serialize(serializer& s) -> void {
    s.array(buffer, size * 3);
  }

private:
  T buffer[size * 3] = {0};
};
