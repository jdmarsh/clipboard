#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>

namespace cb {
    std::string getClipboardData();
    void setClipboardData(const std::string &);
}