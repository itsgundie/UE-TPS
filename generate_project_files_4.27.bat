@echo off
call "%~dp0DevOps\config.bat"

devops\misc\generate_project_files.bat "%EnginePath_4.27%" "%UBTRelativePathV4%" "%VersionSelector%" "%ProjectName%"
