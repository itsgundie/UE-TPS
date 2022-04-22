@echo off

call "%~dp0\..\config.bat"

start "" "%GameExePath%" ?listen -WINDOWED -ResX=1280 -ResY=720 -WinX=1800 -WinY=740 -ExecCmd="open 127.0.0.1"
