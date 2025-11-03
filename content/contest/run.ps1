param([bool]$recompile=$true)
if ($recompile) {
    if (Test-Path -Path './a.exe') {
        Remove-Item ./a.exe
    }
    if (Test-Path -Path './main.txt') {
        Remove-Item ./main.txt
    }
    g++ -std=c++17 -W -Wall -D_GLIBCXX_ASSERTIONS -O3 -g ./main.cpp
}
if (Test-Path -Path './output.txt') {
    Remove-Item ./output.txt
}
if (Test-Path -Path './a.exe') {
    Get-Content ./input.txt | ./a.exe | Tee-Object ./output.txt
    Copy-Item ./main.cpp ./main.txt
}