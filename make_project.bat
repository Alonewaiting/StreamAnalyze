set "QtPath6=D:\Qt\6.4.3\msvc2019_64"
set /p "QtPath6=Enter path or just ENTER for default [%QtPath6%] :
cmake -S . -B sln_x64 -G "Visual Studio 16 2019" -A x64  -DCMAKE_BUILD_TYPE=Debug -DBUILD_TARGET=windows -DQT_DIR:STRING=${QtPath6}
