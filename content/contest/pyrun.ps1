if (Test-Path -Path './main.txt') {
    Remove-Item ./main.txt
}
if (Test-Path -Path './output.txt') {
    Remove-Item ./output.txt
}
Get-Content ./input.txt | py ./main.py | Tee-Object ./output.txt
Copy-Item ./main.cpp ./main.txt