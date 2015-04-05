#include <iostream>

#include "Clipboard.h"

int main() {
    std::cout << cb::getClipboardData();
    std::cin.get();
    return 0;
}