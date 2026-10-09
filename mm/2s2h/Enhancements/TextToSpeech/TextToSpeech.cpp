#include "TextToSpeech.h"
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sapi.h>
#pragma comment(lib, "sapi.lib")
#pragma comment(lib, "ole32.lib")

static ISpVoice* sVoice = nullptr;
static bool sTriedInit = false;

static void EnsureInit() {
    if (sTriedInit) {
        return;
    }
    sTriedInit = true;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    // RPC_E_CHANGED_MODE just means COM was already set up on this thread; that's fine.
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
        return;
    }
    if (FAILED(CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice,
                                reinterpret_cast<void**>(&sVoice)))) {
        sVoice = nullptr;
    }
}
#endif

extern "C" void TextToSpeech_Speak(const char* text) {
#ifdef _WIN32
    if (text == nullptr || *text == '\0') {
        return;
    }
    EnsureInit();
    if (sVoice == nullptr) {
        return;
    }
    int n = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (n <= 0) {
        return;
    }
    std::wstring wide(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wide.data(), n);
    sVoice->Speak(wide.c_str(), SPF_ASYNC | SPF_PURGEBEFORESPEAK, nullptr);
#endif
}

extern "C" void TextToSpeech_Stop(void) {
#ifdef _WIN32
    if (sVoice != nullptr) {
        sVoice->Speak(L"", SPF_ASYNC | SPF_PURGEBEFORESPEAK, nullptr);
    }
#endif
}
