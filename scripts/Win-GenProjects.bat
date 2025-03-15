@echo off
pushd ..\
CALL vendor\bin\premake\premake5.exe vs2022
popd
PAUSE