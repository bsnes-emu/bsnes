#ifndef NALL_BASE64_HPP
#define NALL_BASE64_HPP

#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>

namespace nall {

struct Base64 {
  enum class Format : unsigned { MIME, URI };

  inline static string encode(const uint8_t* data, unsigned size, Format format = Format::MIME);
  inline static string encode(const vector<uint8_t>& buffer, Format format = Format::MIME);
  inline static string encode(const string& text, Format format = Format::MIME);

  inline static vector<uint8_t> decode(const string& text);

private:
  inline static void table(char* data, Format format);
  inline static uint8_t value(char data);
};

string Base64::encode(const uint8_t* data, unsigned size, Format format) {
  vector<uint8_t> result;

  char lookup[65];
  table(lookup, format);

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
      result.last() = lookup[buffer];
      buffer = (data[i] & 15) << 2;
      result.append(lookup[buffer]);
      break;

    case 2:
      buffer |= data[i] >> 6;
      result.last() = lookup[buffer];
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

string Base64::encode(const vector<uint8_t>& buffer, Format format) {
  return encode(buffer.data(), buffer.size(), format);
}

string Base64::encode(const string& text, Format format) {
  return encode((const uint8_t*)text.data(), text.size(), format);
}

vector<uint8_t> Base64::decode(const string& text) {
  vector<uint8_t> result;

  uint8_t buffer, output;
  for(unsigned i = 0; i < text.size(); i++) {
    uint8_t buffer = value(text[i]);
    if(buffer == 0) break;

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

void Base64::table(char* data, Format format) {
  for(unsigned n = 0; n < 26; n++) data[ 0 + n] = 'A' + n;
  for(unsigned n = 0; n < 26; n++) data[26 + n] = 'a' + n;
  for(unsigned n = 0; n < 10; n++) data[52 + n] = '0' + n;

  switch(format) {
  case Format::MIME:
    data[62] = '+';
    data[63] = '/';
    data[64] = '=';
    break;

  case Format::URI:
    data[62] = '-';
    data[63] = '_';
    data[64] = 0;
    break;
  }
}

uint8_t Base64::value(char n) {
  if(n >= 'A' && n <= 'Z') return n - 'A' +  0;
  if(n >= 'a' && n <= 'z') return n - 'a' + 26;
  if(n >= '0' && n <= '9') return n - '0' + 52;
  if(n == '+' || n == '-') return 62;
  if(n == '/' || n == '_') return 63;
  return 0;
}

}

#endif
