#pragma once

namespace nall { namespace Encode {

inline auto Base64(const void* vdata, unsigned size, const string& format = "MIME") -> string {
  auto data = (const uint8_t*)vdata;
  vector<uint8_t> result;

  char lookup[65];
  for(unsigned n = 0; n < 26; n++) lookup[ 0 + n] = 'A' + n;
  for(unsigned n = 0; n < 26; n++) lookup[26 + n] = 'a' + n;
  for(unsigned n = 0; n < 10; n++) lookup[52 + n] = '0' + n;

  if(format == "MIME") {
    lookup[62] = '+';
    lookup[63] = '/';
    lookup[64] = '=';
  } else if(format == "URI") {
    lookup[62] = '-';
    lookup[63] = '_';
    lookup[64] = 0;
  } else return "";

  unsigned overflow = (3 - (size % 3)) % 3;  //bytes to round to nearest multiple of 3
  uint8_t buffer;

  for(unsigned i = 0; i < size; i++) {
    switch(i % 3) {
    case 0:
      buffer = data[i] >> 2;
      result.append(lookup[buffer]);
      buffer = (data[i] & 3) << 4;
      result.append(lookup[buffer]);
      break;

    case 1:
      buffer |= data[i] >> 4;
      result.right() = lookup[buffer];
      buffer = (data[i] & 15) << 2;
      result.append(lookup[buffer]);
      break;

    case 2:
      buffer |= data[i] >> 6;
      result.right() = lookup[buffer];
      buffer = (data[i] & 63);
      result.append(lookup[buffer]);
      break;
    }
  }

  if(lookup[64]) {
    if(overflow >= 1) result.append(lookup[64]);
    if(overflow >= 2) result.append(lookup[64]);
  }

  return result;
}

inline auto Base64(const vector<uint8_t>& buffer, const string& format = "MIME") -> string {
  return Base64(buffer.data(), buffer.size(), format);
}

inline auto Base64(const string& text, const string& format = "MIME") -> string {
  return Base64(text.data(), text.size(), format);
}

}}
