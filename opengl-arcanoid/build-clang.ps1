#!/usr/bin/env pwsh
${arch}    = (Get-CimInstance Win32_operatingsystem).OSArchitecture.split('-')[0]
${libPath} = (Join-Path $PSScriptRoot "lib${arch}")
${binPath} = (Join-Path $PSScriptRoot "bin")
${includePath} = (Join-Path $PSScriptRoot "include")

Copy-Item ${libPath}/freeglut.dll ${binPath}/

# clang --std=c++17 -O3 -DNDEBUG -c -o ${binPath}/arcanoid.o ar.cpp
# clang --std=c++17 -O3 -DNDEBUG -c -o ${binPath}/ball.o ball.cpp
# clang --std=c++17 -O3 -DNDEBUG -o ${binPath}/arcanoid.exe ${binPath}/arcanoid.o ${binPath}/ball.o -L"${libPath}" -lfreeglut -lopengl32 -mwindows

clang --std=c++17 -O3 -DNDEBUG -o ${binPath}/arcanoid.exe src/*.cpp -I "${includePath}" -L"${libPath}" -lfreeglut -lopengl32
