@echo off
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src\*.cpp -o ecommerce.exe
if %errorlevel% equ 0 (
    echo [OK] Build thanh cong. Dang chay...
    .\ecommerce.exe
) else (
    echo [LOI] Build that bai.
    pause
)
