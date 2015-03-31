#include "Clipboard.h"

std::string getClipboardData() {
    std::string clipboardText;
    if (OpenClipboard(nullptr)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char *hText = static_cast<char *>(GlobalLock(hData));
        if (hText != nullptr) {
            clipboardText.assign(hText);
        }
        GlobalUnlock(hData);
    }
    CloseClipboard();
    return clipboardText;
}