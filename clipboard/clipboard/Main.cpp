#include <iostream>

#include "Clipboard.h"

int main() {
    setClipboardData("https://github.com/jdmarsh/clipboard");
    std::cout << getClipboardData();
    std::cin.get();
    return 0;
}