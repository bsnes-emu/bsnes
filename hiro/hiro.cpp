#ifndef HIRO_CPP
#define HIRO_CPP

#define foreach(T, ...) ([&] { for(auto& $ : T) { __VA_ARGS__; }}())

#include "components.hpp"
#include "core/core.cpp"
#include "extension/extension.cpp"

#endif
