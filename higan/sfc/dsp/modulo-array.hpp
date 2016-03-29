template<typename T, uint size>
struct ModuloArray {
  inline auto operator[](int index) const -> T {
    return buffer[size + index];
  }

  inline auto read(int index) const -> T {
    return buffer[size + index];
  }

  inline auto write(uint index, const T value) -> void {
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
