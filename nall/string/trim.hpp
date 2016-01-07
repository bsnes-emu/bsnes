#pragma once

namespace nall {

auto string::trim(rstring lhs, rstring rhs, long limit) -> string& {
  rtrim(rhs, limit);
  ltrim(lhs, limit);
  return *this;
}

auto string::ltrim(rstring lhs, long limit) -> string& {
  if(lhs.size() == 0) return *this;
  long matches = 0;
  while(matches < limit) {
    signed offset = lhs.size() * matches;
    signed length = (signed)size() - offset;
    if(length < (signed)lhs.size()) break;
    if(memory::compare(data() + offset, lhs.data(), lhs.size()) != 0) break;
    matches++;
  }
  if(matches) remove(0, lhs.size() * matches);
  return *this;
}

auto string::rtrim(rstring rhs, long limit) -> string& {
  if(rhs.size() == 0) return *this;
  long matches = 0;
  while(matches < limit) {
    signed offset = (signed)size() - rhs.size() * (matches + 1);
    signed length = (signed)size() - offset;
    if(offset < 0 || length < (signed)rhs.size()) break;
    if(memory::compare(data() + offset, rhs.data(), rhs.size()) != 0) break;
    matches++;
  }
  if(matches) resize(size() - rhs.size() * matches);
  return *this;
}

auto string::itrim(rstring lhs, rstring rhs, long limit) -> string& {
  irtrim(rhs, limit);
  iltrim(lhs, limit);
  return *this;
}

auto string::iltrim(rstring lhs, long limit) -> string& {
  if(lhs.size() == 0) return *this;
  long matches = 0;
  while(matches < limit) {
    signed offset = lhs.size() * matches;
    signed length = (signed)size() - offset;
    if(length < (signed)lhs.size()) break;
    if(memory::icompare(data() + offset, lhs.data(), lhs.size()) != 0) break;
    matches++;
  }
  if(matches) remove(0, lhs.size() * matches);
  return *this;
}

auto string::irtrim(rstring rhs, long limit) -> string& {
  if(rhs.size() == 0) return *this;
  long matches = 0;
  while(matches < limit) {
    signed offset = (signed)size() - rhs.size() * (matches + 1);
    signed length = (signed)size() - offset;
    if(offset < 0 || length < (signed)rhs.size()) break;
    if(memory::icompare(data() + offset, rhs.data(), rhs.size()) != 0) break;
    matches++;
  }
  if(matches) resize(size() - rhs.size() * matches);
  return *this;
}

auto string::strip() -> string& {
  rstrip();
  lstrip();
  return *this;
}

auto string::lstrip() -> string& {
  unsigned length = 0;
  while(length < size()) {
    char input = operator[](length);
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    length++;
  }
  if(length) remove(0, length);
  return *this;
}

auto string::rstrip() -> string& {
  unsigned length = 0;
  while(length < size()) {
    bool matched = false;
    char input = operator[](size() - length - 1);
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    length++;
  }
  if(length) resize(size() - length);
  return *this;
}

}
