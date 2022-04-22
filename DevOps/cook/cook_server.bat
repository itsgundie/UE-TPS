@echo off

call "%~dp0\..\config.bat"


"%RunUATPath%" BuildCookRun ^
-project=D:\Jenkins\Jobs\UE_TPS\JenkinsTPS.uproject ^
-platform="%Platform%" ^
-serverconfig="%Configuration%" ^
-build -cook -skippackage -server -noclient