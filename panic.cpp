#include "panic.h"

void panic(const std::string &message) {
    std::cout << message << "\n";
    throw std::exception();
}
