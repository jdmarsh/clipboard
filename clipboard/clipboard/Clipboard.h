#include <string>
#include <vector>
#include <Windows.h>

namespace cb {
	std::string getClipboardData();
	void setClipboardData(const std::string &);
}