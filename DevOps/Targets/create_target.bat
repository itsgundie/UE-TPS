@echo off

del /q "%TargetFilePath%"

for /f "usebackq tokens=*" %%a in ("%TargetTemplateFilePath%") do (
     if %%a == NEW_LINE (
        echo.>>"%TargetFilePath%"
    ) else (
        call echo %%a>>"%TargetFilePath%"
    )
)

call "%~dp0\..\Misc\format_all_files_to_code_style.bat"