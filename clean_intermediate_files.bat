@echo off

call "%~dp0\.\config.bat"

pushd "%ProjectRoot"
FOR %%f in (%dirsToRemove%) do (
	rmdir /s /q %%f
)

FOR %%f in (%filesToRemove%) do (
	del /q %%f
)
popd