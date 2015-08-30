#if defined(Hiro_Image)

Image::Image() {
}

Image::Image(const Image& source) {
  operator=(source);
}

Image::Image(const string& source) {
  nall::image image{source};
  image.transform();
  setImage((const uint32_t*)image.data(), Size{(signed)image.width(), (signed)image.height()});
}

Image::Image(const vector<uint8_t>& source) {
  nall::image image{source};
  image.transform();
  setImage((const uint32_t*)image.data(), Size{(signed)image.width(), (signed)image.height()});
}

Image::Image(const uint32_t* data, Size size) {
  setImage(data, size);
}

Image::~Image() {
  reset();
}

Image::operator bool() const {
  return state.data;
}

auto Image::operator=(const Image& source) -> type& {
  reset();
  if(state.size = source.state.size) {
    state.data = new uint32_t[state.size.width() * state.size.height()];
    memory::copy(state.data, source.state.data, state.size.width() * state.size.height() * sizeof(uint32_t));
  }
  return *this;
}

auto Image::data() const -> uint32_t* {
  return state.data;
}

auto Image::height() const -> signed {
  return state.size.height();
}

auto Image::reset() -> type& {
  if(state.data) delete[] state.data;
  state.data = nullptr;
  state.size = {};
  return *this;
}

auto Image::setImage(const uint32_t* data, Size size) -> type& {
  reset();
  if(data && size) {
    state.size = size;
    state.data = new uint32_t[size.width() * size.height()];
    memory::copy(state.data, data, size.width() * size.height() * sizeof(uint32_t));
  }
  return *this;
}

auto Image::setSize(Size size) -> type& {
  state.size = size;
  if(state.data) delete[] state.data;
  if(state.size) state.data = new uint32_t[size.width() * size.height()];
  return *this;
}

auto Image::size() const -> Size {
  return state.size;
}

auto Image::width() const -> signed {
  return state.size.width();
}

#endif
