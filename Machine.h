#pragma once

#include <unordered_map>
#include "TypedValue.h"

struct Machine {
    std::unordered_map<std::string, TypedValue> memory{};
};