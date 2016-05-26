Sprite::Sprite(uint width, uint height) : width(width), height(height) {
  pixels = new uint32[width * height]();
}

Sprite::~Sprite() {
  delete[] pixels;
}

auto Sprite::setPixels(const nall::image& image) -> void {
  memory::copy(this->pixels, width * height * sizeof(uint32), image.data(), image.size());
}

auto Sprite::setVisible(bool visible) -> void {
  this->visible = visible;
}

auto Sprite::setPosition(int x, int y) -> void {
  this->x = x;
  this->y = y;
}
