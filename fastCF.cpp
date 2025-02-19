#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>

#define PROGRAM_OPEN "Launched fastCF.\n"
#define PROGRAM_CLOSE "Closing fastCF.\n"
#define FAILED_THREAD "Failed to create thread.\n"
#define LOOP_SLEEP 5

constexpr INPUT LEFT_DOWN  = { INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0, NULL } };
constexpr INPUT LEFT_UP  = { INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0, NULL } };

uint16_t CENTER_X, CENTER_Y;
const uint8_t MIN_INTENSITY = 155;
const uint8_t QUIT_KEY = 'Q';   // CTRL + QUIT_KEY

volatile bool clickHeld = false;
HHOOK mouseHook;

DWORD WINAPI MessageLoop(LPVOID lpParam);
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void chromaSearch(HDC *screenDC, HDC *memoryDC, HBITMAP *hBitmap,
                  BITMAPINFO *bmi) noexcept;
inline void shoot() noexcept;


int main() {

    std::cout << PROGRAM_OPEN;
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetProcessDPIAware();

    // get pixel coords
    CENTER_X = (GetSystemMetrics(SM_CXSCREEN) / 2 + 24);
    CENTER_Y = (GetSystemMetrics(SM_CYSCREEN) / 2 );

    // setup device contexts and colors
    HDC screenDC = NULL;
    HDC memoryDC = NULL;
    HBITMAP hBitmap = NULL;
    BITMAPINFO bmi = {};

    // setup message handler thread
    HANDLE listener = CreateThread(NULL, 0, MessageLoop, NULL, 0, NULL);
    if (listener == NULL) {
        std::cerr << FAILED_THREAD << PROGRAM_CLOSE;
    }

    // int count = 0;
    // int sum = 0;

    // main loop
    while (!((GetAsyncKeyState(VK_CONTROL) & 0x8000) 
           && (GetAsyncKeyState(QUIT_KEY) & 0x8000))) {

            // auto t1 = std::chrono::high_resolution_clock::now();
            // chromaSearch(&screenDC, &memoryDC, &hBitmap, &bmi);
            // auto t2 = std::chrono::high_resolution_clock::now();
            // std::cout << "test function took "
            //           << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
            //           << " microseconds\n";
            // sum += std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
            // count++;

        while (clickHeld) {
            chromaSearch(&screenDC, &memoryDC, &hBitmap, &bmi);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_SLEEP));
    } 

    // double average = (double) sum / count;
    // std::cout << "average of "
    //                   << average
    //                   << " microseconds\n";

    // Cleanup
    TerminateThread(listener, 0);
    WaitForSingleObject(listener, INFINITE);
    CloseHandle(listener);
    
    std::cout << PROGRAM_CLOSE;
    return 0;
}


DWORD WINAPI MessageLoop(LPVOID lpParam) {

    // listen for global mouse events
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, 
                                 GetModuleHandle(NULL), 0);

    MSG msg;
    while (true) {
        // listen for + process messages
        if (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Cleanup
    UnhookWindowsHookEx(mouseHook);
    return 0;
}


// interrupt service routine
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode >= 0) {
        if (wParam == WM_RBUTTONDOWN) {
            clickHeld = true;
            return 1;
        }
        if (wParam == WM_RBUTTONUP) {
            clickHeld = false;
            return 1;
        }
    }

    // Let other mouse events propagate
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}


void chromaSearch(HDC *screenDC, HDC *memoryDC, HBITMAP *hBitmap, 
                  BITMAPINFO *bmi) noexcept {

    *screenDC = ::GetDC(nullptr);
    *memoryDC = CreateCompatibleDC(*screenDC);
    
    // create bitmap size 1, 1
    *hBitmap = CreateCompatibleBitmap(*screenDC, 1, 1);
    SelectObject(*memoryDC, *hBitmap);

    // save pixel data to bitmap
    BitBlt(*memoryDC, 0, 0, 1, 1, *screenDC, CENTER_X, CENTER_Y, SRCCOPY);
    
    // set up win32 BITMAPINFO structure to read from bitmap
    bmi->bmiHeader.biSize = 40;
    bmi->bmiHeader.biWidth = 1;
    bmi->bmiHeader.biHeight = -1;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 24;
    bmi->bmiHeader.biCompression = BI_RGB;

    // read and compare colors
    // learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits
    BYTE pixelData[3];
    if (GetDIBits(*memoryDC, *hBitmap, 0, 1, pixelData, bmi, DIB_RGB_COLORS)) {

        BYTE blue = pixelData[0];
        BYTE green = pixelData[1];
        BYTE red = pixelData[2];

        if (red < (MIN_INTENSITY)
            || green < (MIN_INTENSITY)
            || blue < (MIN_INTENSITY)) 
            ; // shoot();
    }

    // cleanup
    DeleteObject(*hBitmap);
    DeleteDC(*memoryDC);
    ::ReleaseDC(nullptr, *screenDC);
}


inline void shoot() noexcept {
    SendInput(1, const_cast<INPUT*>(&LEFT_DOWN), sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    SendInput(1, const_cast<INPUT*>(&LEFT_UP), sizeof(INPUT));
}
