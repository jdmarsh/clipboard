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

void setClipboardData(const std::string &text) {
    if (OpenClipboard(nullptr)){
        EmptyClipboard();
        HGLOBAL hClipboardMemory = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hClipboardMemory){
            char *hClipboardData = static_cast<char *>(GlobalLock(hClipboardMemory));
            memcpy(hClipboardData, text.c_str(), text.size());
            GlobalUnlock(hClipboardMemory);
            SetClipboardData(CF_TEXT, hClipboardMemory);
        }
        GlobalFree(hClipboardMemory);
    }
    CloseClipboard();
}