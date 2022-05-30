
if (Get-Command conan -ErrorAction SilentlyContinue | Test-Path)
{
    conan install "${PSScriptRoot}" -pr "${Env:USERPROFILE}\.conan\profiles\windows-msvc-17-static-debug-x64"          --build=missing -if ./_conan/debug
    conan install "${PSScriptRoot}" -pr "${Env:USERPROFILE}\.conan\profiles\windows-msvc-17-static-release-x64"        --build=missing -if ./_conan/release
    conan install "${PSScriptRoot}" -pr "${Env:USERPROFILE}\.conan\profiles\windows-msvc-17-static-relwithdebinfo-x64" --build=missing -if ./_conan/relwithdebinfo
}
else
{
    Write-Host "Conan not found. Please install it first. Exiting..." -ForegroundColor Red
    exit 1
}

if (Get-Command premake5 -ErrorAction SilentlyContinue | Test-Path)
{
    premake5 vs2022
}
else
{
    Write-Host "Premake not found. Please install it first. Exiting..."  -ForegroundColor Red
    exit 1
}
