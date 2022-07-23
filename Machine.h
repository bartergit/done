#pragma once
struct Machine {
    std::unordered_map <std::string, TypedValue> memory{};
};

Machine machine;