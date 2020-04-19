// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dll2.h"
#include <map>

std::map<char, int> frequencies;

HBRUSH barBrush = NULL;

int countStringLength(const char* source)
{
    for (int size = 0; true; size++)
    {
        if (source[size] == 0)
        {
            return size;
        }
    }
}

void DrawDiagram(HDC hdc, HWND hwnd, const char* source)
{
    if (barBrush == NULL)
    {
        barBrush = CreateSolidBrush(RGB(255, 0, 0));
    }

    frequencies.clear();

    int sourceSize = countStringLength(source);

    for (int i = 0; i < sourceSize; i++)
    {
        if (source[i] == ' ')
        {
            continue;
        }

        char letter = source[i];
        frequencies[letter]++;
    }

    int counter = 0;
    for (auto it : frequencies)
    {
        int frequencyPercent = it.second * 100 / sourceSize;
        char debugMessage[256];
        sprintf_s(debugMessage, "%c: %d%%\n", it.first, frequencyPercent);
        OutputDebugStringA(debugMessage);

        RECT barRect;
        barRect.left = 10 + 30 * counter;
        barRect.right = 20 + 30 * counter;
        barRect.bottom = 200;
        barRect.top = barRect.bottom - frequencyPercent;
        FillRect(hdc, &barRect, barBrush);

        TextOutA(hdc, barRect.left, barRect.bottom + 10, &it.first, 1);

        char percentageMessage[5] { 0 };
        sprintf_s(percentageMessage, "%d%%", frequencyPercent);
        TextOutA(hdc, barRect.left - 10, barRect.top - 20, percentageMessage, countStringLength(percentageMessage));

        counter++;
    }
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

