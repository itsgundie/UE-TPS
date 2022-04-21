@echo off

call "%~dp0\.\config.bat"


"%RunUATPath%" BuildCookRun ^
-project=D:\Jenkins\Jobs\UE_TPS\JenkinsTPS.uproject ^
-platform="%Platform%" ^
-clientconfig="%Configuration%" ^
-archivedirectory="%ArchivePath" ^
-build -cook -package - stage -archive -pak -allmaps