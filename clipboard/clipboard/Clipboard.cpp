#include "Clipboard.h"

std::string getClipboardHTML(UINT);
std::string getClipboardFiles();
std::string getClipboardText();

std::string cb::getClipboardData() {
    std::string clipboardData;
    if (OpenClipboard(nullptr)) {
        UINT HTMLformat = RegisterClipboardFormat("HTML Format");
        UINT formatList[3] = { HTMLformat, CF_HDROP, CF_TEXT };
        int format = GetPriorityClipboardFormat(formatList, 3);
        if (format == HTMLformat) {
            clipboardData = getClipboardHTML(HTMLformat);
        } else if (format == CF_HDROP) {
            clipboardData = getClipboardFiles();
        } else if (format == CF_TEXT) {
            clipboardData = getClipboardText();
        }
    }
    CloseClipboard();
    return clipboardData;
}

std::string getClipboardHTML(UINT HTMLformat) {
    std::string clipboardHTML;
    HANDLE hData = GetClipboardData(HTMLformat);
    if (hData != nullptr) {
        char *hHTML = static_cast<char *>(GlobalLock(hData));
        if (hHTML != nullptr) {
            clipboardHTML.assign(hHTML);
        }
        GlobalUnlock(hData);
    }
    return clipboardHTML;
}

std::string getClipboardFiles() {
    std::string clipboardFiles;
    HANDLE hData = GetClipboardData(CF_HDROP);
    if (hData != nullptr) {
        HDROP hDropData = static_cast<HDROP>(GlobalLock(hData));
        int fileCount = DragQueryFile(hDropData, -1, nullptr, 0);

        for (int i = 0; i < fileCount; ++i) {
            clipboardFiles.append(i == 0 ? "" : "\r\n");
            int filenameSize = DragQueryFile(hDropData, i, NULL, 0);
            char *filename = new char[filenameSize + 1];
            DragQueryFile(hDropData, i, filename, filenameSize + 1);
            clipboardFiles.append(filename);
            delete[] filename;
        }
        GlobalUnlock(hData);
    }
    return clipboardFiles;
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

void cb::setClipboardData(const std::string &text) {
    EmptyClipboard();
    HGLOBAL hClipboardMemory = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hClipboardMemory) {
        char *hClipboardData = static_cast<char *>(GlobalLock(hClipboardMemory));
        memcpy(hClipboardData, text.c_str(), text.size());
        GlobalUnlock(hClipboardMemory);
        SetClipboardData(CF_TEXT, hClipboardMemory);
    }
    GlobalFree(hClipboardMemory);
    CloseClipboard();
}