$log = Join-Path $PSScriptRoot "qt_verify.txt"
$lines = @()
function Add($s) { $lines += $s }

Add "=== $(Get-Date) ==="

$paths = @{
    qmake = "C:\Qt\6.8.3\mingw_64\bin\qmake.exe"
    gpp   = "C:\Qt\Tools\mingw1310_64\bin\g++.exe"
    cmake = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
    ninja = "C:\Qt\Tools\Ninja\ninja.exe"
    windeploy = "C:\Qt\6.8.3\mingw_64\bin\windeployqt.exe"
}

foreach ($k in $paths.Keys) {
    $p = $paths[$k]
    Add "$k : $(if (Test-Path $p) { 'OK' } else { 'MISSING' }) -> $p"
}

foreach ($k in @('qmake','gpp','cmake')) {
    $p = $paths[$k]
    if (Test-Path $p) {
        Add "--- $k version ---"
        Add (& $p --version 2>&1 | Out-String).Trim()
        if ($k -eq 'qmake') { Add (& $p -v 2>&1 | Out-String).Trim() }
    }
}

Add "--- Qt6Widgets ---"
$w = "C:\Qt\6.8.3\mingw_64\lib\cmake\Qt6Widgets\Qt6WidgetsConfig.cmake"
Add $(if (Test-Path $w) { "OK" } else { "MISSING" })

Add "--- DndDice project ---"
Add "DiceRoller.h: $(Test-Path (Join-Path $PSScriptRoot 'src\DiceRoller.h'))"
Add "Ui.cpp: $(Test-Path (Join-Path $PSScriptRoot 'src\Ui.cpp'))"
Add "DndDice.exe: $(Test-Path (Join-Path $PSScriptRoot 'DndDice.exe'))"

$lines | Out-File -FilePath $log -Encoding utf8
