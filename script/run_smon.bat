@echo off

rem visual studio environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

rem return to current batch file directory
cd %~dp0

rem compile
call MSBuild.exe smon_native.sln /t:"smon_native:Rebuild" /p:Platform=x64;Configuration=Release

