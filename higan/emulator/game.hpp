#pragma once

namespace Emulator {

struct Game {
  struct Memory;
  struct Oscillator;

  inline auto load(string_view) -> void;
  inline auto memory(Markup::Node) -> maybe<Memory>;
  inline auto oscillator(natural = 0) -> maybe<Oscillator>;

  struct Memory {
    explicit operator bool() const { return type; }
    inline auto name() const -> string;

    string type;
    boolean battery;
    natural size;
    string content;
    string manufacturer;
    string architecture;
    string identifier;
  };

  struct Oscillator {
    explicit operator bool() const { return frequency; }

    natural frequency;
  };

  Markup::Node document;
  string sha256;
  string label;
  string name;
  string region;
  string revision;
  string board;
  vector<Memory> memoryList;
  vector<Oscillator> oscillatorList;
};

auto Game::load(string_view text) -> void {
  document = BML::unserialize(text);

  sha256 = document["game/sha256"].text();
  label = document["game/label"].text();
  name = document["game/name"].text();
  region = document["game/region"].text();
  revision = document["game/revision"].text();
  board = document["game/board"].text();

  for(auto node : document.find("game/board/memory")) {
    Memory memory;
    memory.type = node["type"].text();
    memory.battery = (bool)node["type/battery"];
    memory.size = node["size"].natural();
    memory.content = node["content"].text();
    memory.manufacturer = node["manufacturer"].text();
    memory.architecture = node["architecture"].text();
    memory.identifier = node["identifier"].text();
    memoryList.append(memory);
  }

  for(auto node : document.find("game/board/oscillator")) {
    Oscillator oscillator;
    oscillator.frequency = node["frequency"].natural();
    oscillatorList.append(oscillator);
  }
}

auto Game::memory(Markup::Node node) -> maybe<Memory> {
  if(!node) return nothing;
  for(auto& memory : memoryList) {
    auto type = node["type"].text();
    auto size = node["size"].natural();
    auto content = node["content"].text();
    auto manufacturer = node["manufacturer"].text();
    auto architecture = node["architecture"].text();
    auto identifier = node["identifier"].text();
    if(type && type != memory.type) continue;
    if(size && size != memory.size) continue;
    if(content && content != memory.content) continue;
    if(manufacturer && manufacturer != memory.manufacturer) continue;
    if(architecture && architecture != memory.architecture) continue;
    if(identifier && identifier != memory.identifier) continue;
    return memory;
  }
  return nothing;
}

auto Game::oscillator(natural index) -> maybe<Oscillator> {
  if(index < oscillatorList.size()) return oscillatorList[index];
  return nothing;
}

auto Game::Memory::name() const -> string {
  if(manufacturer) return string{manufacturer, ".", content, ".", type}.downcase();
  return string{content, ".", type}.downcase();
}

}
