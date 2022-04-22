@echo off

call "%~dp0\..\config.bat"

start "" "%ClientExePath%" -WINDOWED -ResX=1280 -ResY=720 -WinX=10 -WinY=50 -ExecCmds="open 127.0.0.1:7777"
start "" "%ClientExePath%" -WINDOWED -ResX=1280 -ResY=720 -WinX=1291 -WinY=50 -ExecCmds="open 127.0.0.1:7777"