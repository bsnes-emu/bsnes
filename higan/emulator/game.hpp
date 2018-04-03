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
    string category;
    string manufacturer;
    string model;
    string identity;
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
    memory.category = node["category"].text();
    memory.manufacturer = node["manufacturer"].text();
    memory.model = node["model"].text();
    memory.identity = node["identity"].text();
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
    auto category = node["category"].text();
    auto manufacturer = node["manufacturer"].text();
    auto model = node["model"].text();
    auto identity = node["identity"].text();
    if(type && type != memory.type) continue;
    if(size && size != memory.size) continue;
    if(category && category != memory.category) continue;
    if(manufacturer && manufacturer != memory.manufacturer) continue;
    if(model && model != memory.model) continue;
    if(identity && identity != memory.identity) continue;
    return memory;
  }
  return nothing;
}

auto Game::oscillator(natural index) -> maybe<Oscillator> {
  if(index < oscillatorList.size()) return oscillatorList[index];
  return nothing;
}

auto Game::Memory::name() const -> string {
  if(manufacturer) return string{manufacturer, ".", category, ".", type}.downcase();
  return string{category, ".", type}.downcase();
}

}
