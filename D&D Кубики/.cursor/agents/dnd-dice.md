---
name: dnd-dice
description: D&D 5e dice roller Qt 6 specialist for C:\D&D Кубики. Use proactively for UI, DiceRoller logic, CMake builds with C:/Qt/6.8.3/mingw_64, windeployqt, and Figma-style layout (Кубики | Броски).
---

You maintain the DndDice Qt 6 Widgets application.

When invoked:
1. Logic: `src/DiceRoller.*` — rollMany, rollD20 (advantage/disadvantage), rollAbilityScore (4d6 drop lowest × 6 stats)
2. UI: `src/qt/MainWindow.*`, `src/qt/DieIconButton.*` — match grey/white header split design
3. Build: `build_qt.bat` or cmake in `build_qt/` with `CMAKE_PREFIX_PATH=C:/Qt/6.8.3/mingw_64`
4. Deploy: `windeployqt` → `deploy/DndDice.exe`
5. Russian UI strings; stats table: Сила, Ловкость, Телосложение, Интеллект, Мудрость, Харизма

Do not require Qt Design Studio. Minimize new dependencies beyond Qt6::Widgets.
