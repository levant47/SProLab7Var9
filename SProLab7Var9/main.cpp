#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <functional>
#include <string>

#include "../SProLab7Var9DLL2/dll2.h"

HINSTANCE hInst;
LPCTSTR szWindowClass = "WindowClass";
LPCTSTR szTitle = "Title";

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const char* DLL1Name = "SProLab7Var9DLL1";
HMODULE DLL1;

HWND inputTextBox;
HWND outputTextBox;

typedef void(*ReverseStringInTextBoxType)(HWND,HWND);
ReverseStringInTextBoxType ReverseStringInTextBox;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DLL1 = LoadLibrary(DLL1Name);
    if (DLL1 == NULL)
    {
        MessageBox(NULL, "Failed to load DLL #1", "Error", MB_OK);
        return FALSE;
    }

    ReverseStringInTextBox = (ReverseStringInTextBoxType)GetProcAddress(DLL1, "ReverseStringInTextBox");
    if (ReverseStringInTextBox == NULL)
    {
        MessageBox(NULL, "Failed to load function 'ReverseStringInTextBox' from DLL #1", "Error", MB_OK);
        return FALSE;
    }

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
} 

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_HAND);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    hInst = hInstance;
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
        case WM_CREATE:
        {
            inputTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                50, 10, 200, 25, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
            outputTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                50, 45, 200, 25, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
            break;
        }
        case WM_COMMAND:
        {
            HWND control = (HWND)lParam;
            int message = HIWORD(wParam);
            if (control == inputTextBox && message == WM_USER)
            {
                ReverseStringInTextBox(inputTextBox, outputTextBox);
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            HDC hdc = BeginPaint(hWnd, &paintStruct);

            char inputTitle[] = "Input:";
            TextOut(hdc, 10, 15, inputTitle, sizeof(inputTitle));
            char outputTitle[] = "Output:";
            TextOut(hdc, 0, 50, outputTitle, sizeof(outputTitle));

            char inputText[256];
            GetWindowText(inputTextBox, inputText, 256);
            DrawDiagram(hdc, hWnd, inputText);

            EndPaint(hWnd, &paintStruct);

            break;
        }
        case WM_DESTROY:
            FreeLibrary(DLL1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
