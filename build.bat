@echo off
echo Building multi-file project...
C:\msys64\msys2_shell.cmd -defterm -here -no-start -ucrt64 -c "g++ '/c/Users/HP/Desktop/code hust/prj/'*.cpp -o '/c/Users/HP/Desktop/code hust/prj/PRJ.exe' -std=c++17 2>&1 && echo [OK] Build success! || echo [FAIL] Build failed!"
pause
