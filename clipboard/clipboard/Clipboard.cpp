#include "Clipboard.h"

std::string getClipboardText();
std::string getClipboardFiles();

std::string cb::getClipboardData() {
    std::string clipboardData;
    if (OpenClipboard(nullptr)) {
        UINT formatList[2] = { CF_HDROP, CF_TEXT };
        int format = GetPriorityClipboardFormat(formatList, 2);
        if (format > 0) {
            switch (format) {
            case CF_TEXT:
                clipboardData = getClipboardText();
                break;
            case CF_HDROP:
                clipboardData = getClipboardFiles();
                break;
            }
        }
    }
    CloseClipboard();
    return clipboardData;
}

std::string getClipboardText() {
    std::string clipboardText;
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData != nullptr) {
        char *hText = static_cast<char *>(GlobalLock(hData));
        if (hText != nullptr) {
            clipboardText.assign(hText);
        }
        GlobalUnlock(hData);
    }
    return clipboardText;
}

std::string getClipboardFiles() {
    std::string clipboardText;
    HANDLE hData = GetClipboardData(CF_HDROP);
    if (hData != nullptr) {
        HDROP hDropData = static_cast<HDROP>(GlobalLock(hData));
        int fileCount = DragQueryFile(hDropData, -1, nullptr, 0);

        for (int i = 0; i < fileCount; ++i) {
            clipboardText.append(i == 0 ? "" : "\r\n");
            int filenameSize = DragQueryFile(hDropData, i, NULL, 0);
            char *filename = new char[filenameSize + 1];
            DragQueryFile(hDropData, i, filename, filenameSize + 1);
            clipboardText.append(filename);
            delete[] filename;
        }
        GlobalUnlock(hData);
    }
    return clipboardText;
}

void cb::setClipboardData(const std::string &text) {
    EmptyClipboard();
    HGLOBAL hClipboardMemory = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hClipboardMemory){
        char *hClipboardData = static_cast<char *>(GlobalLock(hClipboardMemory));
        memcpy(hClipboardData, text.c_str(), text.size());
        GlobalUnlock(hClipboardMemory);
        SetClipboardData(CF_TEXT, hClipboardMemory);
    }
    GlobalFree(hClipboardMemory);
    CloseClipboard();
}