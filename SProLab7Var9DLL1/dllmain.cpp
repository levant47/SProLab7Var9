// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>

std::string source;
std::string result;

extern "C" void __declspec(dllexport) ReverseStringInTextBox(HWND inputTextBox, HWND outputTextBox)
{
    char buffer[256] { 0 };
    GetWindowTextA(inputTextBox, buffer, 256);
    source = buffer;
    result = "";

    for (int i = source.length() - 1; i >= 0; i--)
    {
        result += source[i];
    }

    SetWindowTextA(outputTextBox, result.c_str());
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

