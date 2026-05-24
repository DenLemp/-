#pragma once
#include <windows.h>

void registerDieButtonClass(HINSTANCE inst);
HWND createDieButton(HWND parent, int id, int sides, int x, int y, int size);
