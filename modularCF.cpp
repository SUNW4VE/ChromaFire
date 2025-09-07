#include <windows.h>
#include <atomic>
#include <thread>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#define CACHE_ALIGNED __attribute__((aligned(64)))

struct sys_state {
    std::atomic<bool> clickHeld{false};

    uint16_t CENTER[2];
    uint8_t MIN_INTEN = 155;
    uint8_t QUIT_KEY = 'Q';

    HHOOK mouseHook;
} CACHE_ALIGNED;

static struct scan_operations cf_ops {
    .coords = cf_coords_init,
    .fetch_loop = sys_fetch_message_loop,
    .submit = cf_submit,
    .process = cf_search,
    .act = cf_shoot,
};

static struct sys_operations sys_ops {
    .proc_init = cf_os_init,
    .thread_init = cf_thread_init,
    .handle_event = sys_process_hook,
};

enum __attribute__((packed)) cf_events {
    SHOOT = 0,
    HOLD = (1 << 0)
};

enum __attribute__((packed)) coords {
    X = 0,
    Y = (1 << 0)
};

struct cf_message {
    INPUT input;
    uint8_t req;
    uint8_t inten; 
    
    void *opaque;
} CACHE_ALIGNED;

struct cf_context {
    HANDLE *thread;

    struct cf_message[64];
} CACHE_ALIGNED;

constexpr INPUT LEFT_DOWN     = { INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0, NULL } };
constexpr INPUT LEFT_UP       = { INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0, NULL } };

DWORD WINAPI sys_fetch_message_loop(LPVOID lpParam);
LRESULT CALLBACK sys_process_hook(int nCode, WPARAM wParam, LPARAM lParam);

int cf_os_init();
HANDLE cf_thread_init();
int cf_coords_init();
void cf_search(HDC *screenDC, HDC *memoryDC, HBITMAP *hBitmap,
                  BITMAPINFO *bmi) noexcept;
inline void cf_shoot() noexcept;


int main() {

    sys_ops->proc_init();
    cf_ops->coords();
    sys_ops->thread_init();

    // setup device contexts and colors
    HDC screenDC    = NULL;
    HDC memoryDC    = NULL;
    HBITMAP hBitmap = NULL;
    BITMAPINFO bmi  = {};

    // setup message handler thread
    HANDLE cf_thread = sys_ops.thread_init();
    if (listener == NULL) {
        std::cerr << "Failed to allocate thread.\n";
        return 0;
    }

    // main loop
    while (unlikely((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(QUIT_KEY) & 0x8000))) {
        while (likely(clickHeld)) {
            chromaSearch(&screenDC, &memoryDC, &hBitmap, &bmi);
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_SLEEP));
    } 

    // Cleanup
    TerminateThread(listener, 0);
    WaitForSingleObject(listener, INFINITE);
    CloseHandle(listener);

    std::cout << "Closing ChromaFire.\n";
    return 0;
}

int cf_os_init() {
    std::cout << "Launched ChromaFire.\n";
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetProcessDPIAware();

    return 1;
}

HANDLE cf_thread_init() {
    HANDLE listener = CreateThread(NULL, 0, MessageLoop, NULL, 0, NULL);

    if (unlikely(listener == NULL)) {
        std::cerr << "Failed to allocate thread.\n";
        return 0;
    }

    return listener;
}

int cf_coords_init(uint16_t *COORDS) {
    COORDS[X] = GetSystemMetrics(SM_CXSCREEN) >> 1;
    COORDS[Y] = GetSystemMetrics(SM_CYSCREEN) >> 1;

    return 1;
}

int cf_submit(struct *cf_message) {
    return 1;
}

DWORD WINAPI sys_fetch_message_loop(LPVOID lpParam) {

    // listen for global mouse events
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, 
                                 GetModuleHandle(NULL), 0);

    MSG msg;
    while (likely(1)) {
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
LRESULT CALLBACK sys_process_hook(int nCode, WPARAM wParam, LPARAM lParam) {

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


static void cf_search(HDC *screenDC, HDC *memoryDC, HBITMAP *hBitmap, 
                  BITMAPINFO *bmi) noexcept {

    *screenDC = ::GetDC(nullptr);
    *memoryDC = CreateCompatibleDC(*screenDC);
    
    // create bitmap size 1, 1
    *hBitmap = CreateCompatibleBitmap(*screenDC, 1, 1);
    SelectObject(*memoryDC, *hBitmap);

    // save pixel data to bitmap
    BitBlt(*memoryDC, 0, 0, 1, 1, *screenDC, CENTER_X, CENTER_Y, SRCCOPY);
    
    // set up win32 BITMAPINFO structure to read from bitmap
    bmi->bmiHeader.biSize           = 40;
    bmi->bmiHeader.biWidth          = 1;
    bmi->bmiHeader.biHeight         = -1;
    bmi->bmiHeader.biPlanes         = 1;
    bmi->bmiHeader.biBitCount       = 24;
    bmi->bmiHeader.biCompression    = BI_RGB;

    // read and compare colors
    // learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits
    BYTE pixelData[3];
    if (GetDIBits(*memoryDC, *hBitmap, 0, 1, pixelData, bmi, DIB_RGB_COLORS)) {

        BYTE blue   = pixelData[0];
        BYTE green  = pixelData[1];
        BYTE red    = pixelData[2];

        if (red < (MIN_INTENSITY) 
            || green < (MIN_INTENSITY) 
            || blue < (MIN_INTENSITY)) 
                shoot();
    }

    // cleanup
    DeleteObject(*hBitmap);
    DeleteDC(*memoryDC);
    ::ReleaseDC(nullptr, *screenDC);
}

inline void cf_shoot() noexcept {
    SendInput(1, const_cast<INPUT*>(&LEFT_DOWN), sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    SendInput(1, const_cast<INPUT*>(&LEFT_UP), sizeof(INPUT));
}
