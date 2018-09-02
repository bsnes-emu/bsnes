#pragma once

#include <nall/random.hpp>
#include <nall/cipher/chacha20.hpp>
#include <nall/elliptic-curve/ed25519.hpp>
#include <nall/encode/base.hpp>
#include <nall/decode/base.hpp>
#include <nall/encode/lzsa.hpp>
#include <nall/decode/lzsa.hpp>

namespace nall { namespace Beat {

struct Archive {
  struct Encryption {
    string type;
    uint256_t key = 0;
    uint192_t nonce = 0;
  };

  struct Signature {
    string type;
    uint256_t privateKey = 0;
    uint256_t publicKey = 0;
    uint512_t signature = 0;
  };

  struct Compression {
    string type;
    uint size = 0;
  };

  //timestamps are human-readable strings in ISO 8601 format; save for T=>space
  //times are stored in UTC, rather than local times
  struct Timestamps {
    string created;
    string modified;
    string accessed;
  };

  struct Permissions {
    string name;
    bool readable = false;
    bool writable = false;
    bool executable = false;
  };

  struct Node {
    string name;

    //paths and files
    Timestamps timestamps;
    struct {
      Permissions owner;
      Permissions group;
      Permissions other;
    } permissions;

    //files only
    uint offset = 0;
    uint size = 0;

    Compression compression;

    string filename;
    vector<uint8_t> filedata;
  };

  auto append(const Node& node) -> bool;
  auto encryptionManifest() -> string;
  auto manifest() -> string;
  auto create() -> vector<uint8_t>;

  //internal functions
  auto encode() -> vector<uint8_t>;
  auto encode(Node& node, uint64_t offset) -> vector<uint8_t>;

  Encryption encryption;
  Signature signature;
  Compression compression;  //solid archiving
  vector<Node> nodes;
};

auto Archive::append(const Node& node) -> bool {
  //prevent multiple nodes with the same name
  if(nodes.find([&](auto& item) { return item.name == node.name; })) return false;

  nodes.append(node);
  return true;
}

auto Archive::encryptionManifest() -> string {
  string manifest;
  manifest.append("encryption\n");
  manifest.append("  type:  ", encryption.type, "\n");
  manifest.append("  nonce: ", Encode::Base<57>(encryption.nonce), "\n");
  return manifest;
}

auto Archive::manifest() -> string {
  string manifest;
  manifest.append("archive\n");

  for(auto& node : nodes) {
    if(node.name.endsWith("/")) {
      manifest.append("  path: ", string{node.name}.trimRight("/", 1L), "\n");
    } else {
      manifest.append("  file: ", node.name, "\n");
      manifest.append("    offset: ", node.offset, "\n");
      manifest.append("    size: ", node.size, "\n");
      if(node.compression.type) {
        manifest.append("    compression: ", node.compression.type, "\n");
        manifest.append("      size: ", node.compression.size, "\n");
      }
    }
    if(node.timestamps.created || node.timestamps.modified || node.timestamps.accessed) {
      manifest.append("    timestamp\n");
      if(auto timestamp = node.timestamps.created ) manifest.append("      created:  ", timestamp, "\n");
      if(auto timestamp = node.timestamps.modified) manifest.append("      modified: ", timestamp, "\n");
      if(auto timestamp = node.timestamps.accessed) manifest.append("      accessed: ", timestamp, "\n");
    }
    if(node.permissions.owner.name || node.permissions.group.name || node.permissions.other.name) {
      manifest.append("    permission\n");
      if(node.permissions.owner.name) {
        manifest.append("      owner: ", node.permissions.owner.name, "\n");
        if(node.permissions.owner.readable  ) manifest.append("        readable\n");
        if(node.permissions.owner.writable  ) manifest.append("        writable\n");
        if(node.permissions.owner.executable) manifest.append("        executable\n");
      }
      if(node.permissions.group.name) {
        manifest.append("      group: ", node.permissions.group.name, "\n");
        if(node.permissions.group.readable  ) manifest.append("        readable\n");
        if(node.permissions.group.writable  ) manifest.append("        writable\n");
        if(node.permissions.group.executable) manifest.append("        executable\n");
      }
      if(node.permissions.other.name) {
        manifest.append("      other\n");
        if(node.permissions.other.readable  ) manifest.append("        readable\n");
        if(node.permissions.other.writable  ) manifest.append("        writable\n");
        if(node.permissions.other.executable) manifest.append("        executable\n");
      }
    }
  }

  if(compression.type) {
    manifest.append("  compression: ", compression.type, "\n");
    manifest.append("    size: ", compression.size, "\n");
  }

  if(signature.type == "ed25519") {
    manifest.append("  signature: ", signature.type, "\n");
    manifest.append("    publicKey: ", Encode::Base<57>(signature.publicKey), "\n");
    manifest.append("    signature: ", Encode::Base<57>(signature.signature), "\n");
  }

  return manifest;
}

auto Archive::create() -> vector<uint8_t> {
  vector<uint8_t> output;

  output.append('B');
  output.append('P');
  output.append('A');
  output.append('1');

  nodes.sort([&](auto& lhs, auto& rhs) {
    return string::compare(lhs.name, rhs.name) < 0;
  });

  auto content = encode();
  if(compression.type == "lzsa") {
    content = Encode::LZSA(content);
    compression.size = content.size();
  }

  if(signature.type == "ed25519") {
    EllipticCurve::Ed25519 ed25519;
    signature.publicKey = ed25519.publicKey(signature.privateKey);
    signature.signature = ed25519.sign(content, signature.privateKey);
  }

  if(encryption.type == "xchacha20") {
    //a randomly generated nonce is preferred
    if(!encryption.nonce) {
      CSPRNG csprng;
      encryption.nonce = csprng.random<uint192_t>();
    }
    Cipher::XChaCha20 xchacha20{encryption.key, encryption.nonce};
    content = xchacha20.encrypt(content);

    string manifest;
    manifest.append("encryption\n");
    manifest.append("  type:  ", encryption.type, "\n");
    manifest.append("  nonce: ", Encode::Base<57>(encryption.nonce), "\n");

    output.append(content);
    for(uint8_t byte : manifest) output.append(byte);
    output.appendl(manifest.size(), 8);
  } else {
    encryption = {};
    output.append(content);
  }

  auto sha256 = Hash::SHA256(output).value();
  output.appendl(sha256, 32);

  return output;
}

//

auto Archive::encode() -> vector<uint8_t> {
  vector<uint8_t> output;

  for(auto& node : nodes) {
    if(node.filename) {
      node.timestamps.created  = chrono::utc::datetime(inode::timestamp(node.filename, inode::time::create));
      node.timestamps.accessed = chrono::utc::datetime(inode::timestamp(node.filename, inode::time::access));
      node.timestamps.modified = chrono::utc::datetime(inode::timestamp(node.filename, inode::time::modify));

      uint mode = inode::mode(node.filename);
      node.permissions.owner.name = inode::user(node.filename);
      node.permissions.owner.executable = mode & 0100;
      node.permissions.owner.writable   = mode & 0200;
      node.permissions.owner.readable   = mode & 0400;
      node.permissions.group.name = inode::group(node.filename);
      node.permissions.group.executable = mode & 0010;
      node.permissions.group.writable   = mode & 0020;
      node.permissions.group.readable   = mode & 0040;
      node.permissions.other.name = " ";
      node.permissions.other.executable = mode & 0001;
      node.permissions.other.writable   = mode & 0002;
      node.permissions.other.readable   = mode & 0004;
    }

    if(node.name.endsWith("/")) continue;

    auto buffer = encode(node, output.size());
    output.append(buffer);
  }

  auto manifest = this->manifest();
  for(auto byte : manifest) output.append(byte);
  for(auto byte : range(8)) output.append((uint64_t)manifest.size() >> byte * 8);

  return output;
}

auto Archive::encode(Node& node, uint64_t offset) -> vector<uint8_t> {
  node.offset = offset;

  vector<uint8_t> output;

  if(node.filename) {
    output = file::read(node.filename);
  } else {
    output = node.filedata;
  }

  node.size = output.size();

  if(node.compression.type == "lzsa") {
    output = Encode::LZSA(output);
    node.compression.size = output.size();
  } else {
    node.compression = {};
  }

  return output;
}

}}
