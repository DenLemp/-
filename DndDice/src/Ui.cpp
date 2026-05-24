#include "Ui.h"
#include "DiceRoller.h"
#include <commctrl.h>
#include <string>
#include <vector>

#pragma comment(lib, "comctl32.lib")

namespace {
constexpr int IDC_D4 = 101;
constexpr int IDC_D6 = 102;
constexpr int IDC_D8 = 103;
constexpr int IDC_D10 = 104;
constexpr int IDC_D12 = 105;
constexpr int IDC_D20 = 106;
constexpr int IDC_COUNT = 110;
constexpr int IDC_D20_NORMAL = 120;
constexpr int IDC_D20_ADV = 121;
constexpr int IDC_D20_DIS = 122;
constexpr int IDC_STATS = 130;
constexpr int IDC_RESULT = 200;
constexpr int IDC_DETAILS = 201;

constexpr COLORREF CLR_HEADER = RGB(74, 74, 74);
constexpr COLORREF CLR_PANEL = RGB(232, 232, 232);
constexpr COLORREF CLR_TEXT = RGB(26, 26, 26);

HWND g_hwndMain = nullptr;
HWND g_hwndLeft = nullptr;
HWND g_hwndResult = nullptr;
HWND g_hwndDetails = nullptr;
HWND g_hwndCount = nullptr;
HFONT g_fontBig = nullptr;
HFONT g_fontNormal = nullptr;

int getDiceCount() {
    wchar_t buf[16]{};
    GetWindowTextW(g_hwndCount, buf, 16);
    int n = _wtoi(buf);
    if (n < 1) n = 1;
    if (n > 100) n = 100;
    return n;
}

D20Mode getD20Mode() {
    if (!g_hwndLeft) return D20Mode::Normal;
    if (IsDlgButtonChecked(g_hwndLeft, IDC_D20_ADV) == BST_CHECKED)
        return D20Mode::Advantage;
    if (IsDlgButtonChecked(g_hwndLeft, IDC_D20_DIS) == BST_CHECKED)
        return D20Mode::Disadvantage;
    return D20Mode::Normal;
}

std::wstring formatDiceList(const RollResult& r) {
    std::wstring s;
    for (size_t i = 0; i < r.dice.size(); ++i) {
        if (i) s += L", ";
        s += std::to_wstring(r.dice[i]);
    }
    return s;
}

void setResult(const std::wstring& mainText, const std::wstring& details) {
    SetWindowTextW(g_hwndResult, mainText.c_str());
    SetWindowTextW(g_hwndDetails, details.c_str());
}

void onDieRoll(HWND hwnd, int sides) {
    const int count = getDiceCount();
    RollResult r;
    std::wstring details;

    if (sides == 20) {
        const D20Mode mode = getD20Mode();
        if (count == 1) {
            r = DiceRoller::rollD20(mode);
            if (r.dice.size() == 2) {
                const wchar_t* modeStr =
                    mode == D20Mode::Advantage ? L"преимущество" : L"помеха";
                details = L"d20 (" + std::wstring(modeStr) + L"): " + formatDiceList(r) +
                          L" → " + std::to_wstring(r.total);
            } else {
                details = L"1d20: " + std::to_wstring(r.total);
            }
        } else if (mode == D20Mode::Normal) {
            r = DiceRoller::rollMany(20, count);
            details = std::to_wstring(count) + L"d20: [" + formatDiceList(r) + L"]";
        } else {
            r.dice.reserve(static_cast<size_t>(count * 2));
            for (int i = 0; i < count; ++i) {
                RollResult one = DiceRoller::rollD20(mode);
                r.total += one.total;
                for (int v : one.dice) r.dice.push_back(v);
            }
            const wchar_t* modeStr =
                mode == D20Mode::Advantage ? L"преимущество" : L"помеха";
            details = std::to_wstring(count) + L"× d20 (" + modeStr + L"): сумма " +
                      std::to_wstring(r.total);
        }
        setResult(std::to_wstring(r.total), details);
        return;
    }

    r = DiceRoller::rollMany(sides, count);
    if (count == 1) {
        details = L"1d" + std::to_wstring(sides) + L": " + std::to_wstring(r.total);
    } else {
        details = std::to_wstring(count) + L"d" + std::to_wstring(sides) + L": [" +
                  formatDiceList(r) + L"]";
    }
    setResult(std::to_wstring(r.total), details);
}

LRESULT CALLBACK StatsDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM) {
    if (msg == WM_COMMAND && LOWORD(wp) == IDOK) {
        DestroyWindow(dlg);
        return 0;
    }
    if (msg == WM_CLOSE) {
        DestroyWindow(dlg);
        return 0;
    }
    return DefWindowProcW(dlg, msg, wp, 0);
}

void showStatsDialog(HWND owner) {
    static bool reg = false;
    if (!reg) {
        WNDCLASSW wc{};
        wc.lpfnWndProc = StatsDlgProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"DndStatsDlg";
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        RegisterClassW(&wc);
        reg = true;
    }

    RECT prc{};
    GetWindowRect(owner, &prc);
    int dw = 540, dh = 300;
    int x = prc.left + (prc.right - prc.left - dw) / 2;
    int y = prc.top + (prc.bottom - prc.top - dh) / 2;

    HWND dlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"DndStatsDlg",
                               L"Прокид характеристик (6 × 4d6, без низшего)",
                               WS_POPUP | WS_CAPTION | WS_SYSMENU, x, y, dw, dh, owner, nullptr,
                               GetModuleHandle(nullptr), nullptr);
    if (!dlg) return;

    CreateWindowExW(0, L"STATIC",
                    L"Характеристика          Значение          Бросок (4d6, убран низший)",
                    WS_CHILD | WS_VISIBLE, 12, 8, 510, 20, dlg, nullptr, GetModuleHandle(nullptr),
                    nullptr);

    HWND list = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", nullptr,
                                WS_CHILD | WS_VISIBLE | WS_VSCROLL, 12, 32, 510, 200, dlg,
                                reinterpret_cast<HMENU>(1000), GetModuleHandle(nullptr), nullptr);

    const wchar_t* names[] = {L"Сила", L"Ловкость", L"Телосложение",
                              L"Интеллект", L"Мудрость", L"Харизма"};
    for (int i = 0; i < 6; ++i) {
        RollResult r = DiceRoller::rollAbilityScore();
        std::wstring row = std::wstring(names[i]) + L"          " + std::to_wstring(r.total) +
                           L"          4d6: " + formatDiceList(r) + L", убран " +
                           std::to_wstring(r.dropped);
        SendMessageW(list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.c_str()));
    }

    CreateWindowExW(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 220, 242, 100,
                    28, dlg, reinterpret_cast<HMENU>(IDOK), GetModuleHandle(nullptr), nullptr);

    ShowWindow(dlg, SW_SHOW);
    EnableWindow(owner, FALSE);

    MSG msg{};
    while (IsWindow(dlg)) {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                PostQuitMessage(static_cast<int>(msg.wParam));
                break;
            }
            if (msg.hwnd == dlg || IsChild(dlg, msg.hwnd)) {
                if (msg.message == WM_COMMAND && LOWORD(msg.wParam) == IDOK) {
                    DestroyWindow(dlg);
                    continue;
                }
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            } else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }
    }
    EnableWindow(owner, TRUE);
    SetForegroundWindow(owner);
}

void createDieButton(HWND parent, int id, const wchar_t* label, int x) {
    CreateWindowExW(0, L"BUTTON", label, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, 88, 72, 72,
                    parent, reinterpret_cast<HMENU>(id), GetModuleHandle(nullptr), nullptr);
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_CTLCOLORSTATIC || msg == WM_CTLCOLORBTN) {
        HDC hdc = reinterpret_cast<HDC>(wp);
        SetBkColor(hdc, CLR_PANEL);
        SetTextColor(hdc, CLR_TEXT);
        static HBRUSH brush = CreateSolidBrush(CLR_PANEL);
        return reinterpret_cast<LRESULT>(brush);
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT CALLBACK HeaderProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps{};
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc{};
        GetClientRect(hwnd, &rc);
        HBRUSH br = CreateSolidBrush(CLR_HEADER);
        FillRect(hdc, &rc, br);
        DeleteObject(br);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        SelectObject(hdc, g_fontNormal);
        RECT left = rc, right = rc;
        left.right = rc.right / 2;
        right.left = rc.right / 2;
        DrawTextW(hdc, L"Кубики", -1, &left, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DrawTextW(hdc, L"Броски", -1, &right, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        WNDCLASSW wcPanel{};
        wcPanel.lpfnWndProc = PanelProc;
        wcPanel.hInstance = GetModuleHandle(nullptr);
        wcPanel.lpszClassName = L"DndPanel";
        RegisterClassW(&wcPanel);

        WNDCLASSW wcHeader{};
        wcHeader.lpfnWndProc = HeaderProc;
        wcHeader.hInstance = GetModuleHandle(nullptr);
        wcHeader.lpszClassName = L"DndHeader";
        RegisterClassW(&wcHeader);

        RECT rc{};
        GetClientRect(hwnd, &rc);
        int w = rc.right, h = rc.bottom;
        int headerH = 48;
        int half = w / 2;

        CreateWindowExW(0, L"DndHeader", nullptr, WS_CHILD | WS_VISIBLE, 0, 0, w, headerH, hwnd,
                        nullptr, GetModuleHandle(nullptr), nullptr);

        g_hwndMain = hwnd;
        HWND left = CreateWindowExW(0, L"DndPanel", nullptr, WS_CHILD | WS_VISIBLE, 0, headerH,
                                    half, h - headerH, hwnd, nullptr, GetModuleHandle(nullptr),
                                    nullptr);
        g_hwndLeft = left;
        HWND right = CreateWindowExW(0, L"DndPanel", nullptr, WS_CHILD | WS_VISIBLE, half,
                                     headerH, w - half, h - headerH, hwnd, nullptr,
                                     GetModuleHandle(nullptr), nullptr);

        CreateWindowExW(0, L"STATIC", L"Количество кубов:", WS_CHILD | WS_VISIBLE, 16, 16, 160,
                        20, left, nullptr, GetModuleHandle(nullptr), nullptr);
        g_hwndCount = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"1",
                                      WS_CHILD | WS_VISIBLE | ES_NUMBER, 180, 12, 60, 24, left,
                                      reinterpret_cast<HMENU>(IDC_COUNT),
                                      GetModuleHandle(nullptr), nullptr);

        int x = 16;
        createDieButton(left, IDC_D4, L"d4", x);
        x += 80;
        createDieButton(left, IDC_D6, L"d6", x);
        x += 80;
        createDieButton(left, IDC_D8, L"d8", x);
        x += 80;
        createDieButton(left, IDC_D10, L"d10", x);
        x += 80;
        createDieButton(left, IDC_D12, L"d12", x);
        x += 80;
        createDieButton(left, IDC_D20, L"d20", x);

        CreateWindowExW(0, L"BUTTON", L"d20 — режим", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 16, 168,
                        half - 32, 88, left, nullptr, GetModuleHandle(nullptr), nullptr);
        CreateWindowExW(0, L"BUTTON", L"Обычный", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 28,
                        192, 100, 22, left, reinterpret_cast<HMENU>(IDC_D20_NORMAL),
                        GetModuleHandle(nullptr), nullptr);
        CreateWindowExW(0, L"BUTTON", L"Преимущество", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                        28, 216, 140, 22, left, reinterpret_cast<HMENU>(IDC_D20_ADV),
                        GetModuleHandle(nullptr), nullptr);
        CreateWindowExW(0, L"BUTTON", L"Помеха", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 28,
                        240, 100, 22, left, reinterpret_cast<HMENU>(IDC_D20_DIS),
                        GetModuleHandle(nullptr), nullptr);
        CheckDlgButton(left, IDC_D20_NORMAL, BST_CHECKED);

        CreateWindowExW(0, L"BUTTON", L"Прокид характеристик (6×4d6)", WS_CHILD | WS_VISIBLE, 16,
                        268, half - 32, 32, left, reinterpret_cast<HMENU>(IDC_STATS),
                        GetModuleHandle(nullptr), nullptr);

        g_hwndResult = CreateWindowExW(0, L"STATIC", L"—",
                                       WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 40, w - half,
                                       h - headerH - 80, right,
                                       reinterpret_cast<HMENU>(IDC_RESULT),
                                       GetModuleHandle(nullptr), nullptr);
        g_hwndDetails = CreateWindowExW(0, L"STATIC", L"Выберите кубик слева",
                                        WS_CHILD | WS_VISIBLE | SS_CENTER, 16, h - headerH - 72,
                                        w - half - 32, 48, right,
                                        reinterpret_cast<HMENU>(IDC_DETAILS),
                                        GetModuleHandle(nullptr), nullptr);
        SendMessageW(g_hwndResult, WM_SETFONT, reinterpret_cast<WPARAM>(g_fontBig), TRUE);
        return 0;
    }
    case WM_SIZE: {
        RECT rc{};
        GetClientRect(hwnd, &rc);
        int w = rc.right, h = rc.bottom;
        int headerH = 48;
        int half = w / 2;
        HWND header = FindWindowExW(hwnd, nullptr, L"DndHeader", nullptr);
        if (header)
            SetWindowPos(header, nullptr, 0, 0, w, headerH, SWP_NOZORDER);
        int panelIdx = 0;
        for (HWND child = FindWindowExW(hwnd, nullptr, L"DndPanel", nullptr); child;
             child = FindWindowExW(hwnd, child, L"DndPanel", nullptr)) {
            if (panelIdx == 0)
                SetWindowPos(child, nullptr, 0, headerH, half, h - headerH, SWP_NOZORDER);
            else
                SetWindowPos(child, nullptr, half, headerH, w - half, h - headerH, SWP_NOZORDER);
            panelIdx++;
        }
        if (g_hwndResult)
            SetWindowPos(g_hwndResult, nullptr, 0, 24, w - half, h - headerH - 100, SWP_NOZORDER);
        if (g_hwndDetails)
            SetWindowPos(g_hwndDetails, nullptr, 8, h - headerH - 72, w - half - 16, 56,
                         SWP_NOZORDER);
        return 0;
    }
    case WM_COMMAND: {
        const int id = LOWORD(wp);
        switch (id) {
        case IDC_D4:
            onDieRoll(hwnd, 4);
            break;
        case IDC_D6:
            onDieRoll(hwnd, 6);
            break;
        case IDC_D8:
            onDieRoll(hwnd, 8);
            break;
        case IDC_D10:
            onDieRoll(hwnd, 10);
            break;
        case IDC_D12:
            onDieRoll(hwnd, 12);
            break;
        case IDC_D20:
            onDieRoll(hwnd, 20);
            break;
        case IDC_STATS:
            showStatsDialog(hwnd);
            break;
        default:
            break;
        }
        return 0;
    }
    case WM_DESTROY:
        if (g_fontBig) DeleteObject(g_fontBig);
        if (g_fontNormal) DeleteObject(g_fontNormal);
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
} // namespace

namespace Ui {
HWND createMainWindow(HINSTANCE instance) {
    INITCOMMONCONTROLSEX icc{sizeof(icc), ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&icc);

    g_fontNormal = CreateFontW(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                               OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                               DEFAULT_PITCH, L"Segoe UI");
    g_fontBig = CreateFontW(96, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                            DEFAULT_PITCH, L"Segoe UI");

    WNDCLASSW wc{};
    wc.lpfnWndProc = MainProc;
    wc.hInstance = instance;
    wc.lpszClassName = L"DndDiceMain";
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, L"DndDiceMain", L"D&D — Кубики",
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                                920, 420, nullptr, nullptr, instance, nullptr);
    return hwnd;
}

} // namespace
