$log = Join-Path $PSScriptRoot "qt_check.txt"
@(
    "=== where qmake ==="
    (Get-Command qmake -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source)
    "=== qmake -v ==="
    (& qmake -v 2>&1 | Out-String)
    "=== C:\Qt folders ==="
    (Get-ChildItem C:\Qt -Directory -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName })
    "=== qmake paths ==="
    (Get-ChildItem C:\Qt -Recurse -Filter qmake.exe -ErrorAction SilentlyContinue | Select-Object -ExpandProperty FullName)
) | Out-File -FilePath $log -Encoding utf8
