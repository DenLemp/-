#include "DieButton.h"
#include "DieDraw.h"

namespace {
constexpr wchar_t kClassName[] = L"DndDieButton";
int g_sides = 0;

LRESULT CALLBACK DieBtnProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lp);
        g_sides = reinterpret_cast<int>(reinterpret_cast<intptr_t>(cs->lpCreateParams));
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, cs->lpCreateParams);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc{};
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
        int sides = static_cast<int>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        DieDraw::drawDie(hdc, rc, sides);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
        SendMessageW(GetParent(hwnd), WM_COMMAND,
                     MAKEWPARAM(GetDlgCtrlID(hwnd), BN_CLICKED),
                     reinterpret_cast<LPARAM>(hwnd));
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
} // namespace

void registerDieButtonClass(HINSTANCE inst) {
    static bool once = false;
    if (once) return;
    WNDCLASSW wc{};
    wc.lpfnWndProc = DieBtnProc;
    wc.hInstance = inst;
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_HAND);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    RegisterClassW(&wc);
    once = true;
}

HWND createDieButton(HWND parent, int id, int sides, int x, int y, int size) {
    return CreateWindowExW(
        WS_EX_CLIENTEDGE, kClassName, L"", WS_CHILD | WS_VISIBLE, x, y, size, size, parent,
        reinterpret_cast<HMENU>(static_cast<intptr_t>(id)), GetModuleHandle(nullptr),
        reinterpret_cast<void*>(static_cast<intptr_t>(sides)));
}
