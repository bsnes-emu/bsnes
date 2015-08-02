#ifndef NALL_DECODE_BASE64_HPP
#define NALL_DECODE_BASE64_HPP

namespace nall { namespace Decode {

inline auto Base64(const string& text) -> vector<uint8_t> {
  auto value = [](char n) -> uint8_t {
    if(n >= 'A' && n <= 'Z') return n - 'A' +  0;
    if(n >= 'a' && n <= 'z') return n - 'a' + 26;
    if(n >= '0' && n <= '9') return n - '0' + 52;
    if(n == '+' || n == '-') return 62;
    if(n == '/' || n == '_') return 63;
    return 64;  //error code
  };

  vector<uint8_t> result;

  uint8_t buffer, output;
  for(unsigned i = 0; i < text.size(); i++) {
    uint8_t buffer = value(text[i]);
    if(buffer > 63) break;

    switch(i & 3) {
    case 0:
      output = buffer << 2;
      break;

    case 1:
      result.append(output | buffer >> 4);
      output = (buffer & 15) << 4;
      break;

    case 2:
      result.append(output | buffer >> 2);
      output = (buffer & 3) << 6;
      break;

    case 3:
      result.append(output | buffer);
      break;
    }
  }

  return result;
}

}}

#endif
