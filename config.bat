@echo off

rem Engine params
set EnginePath_4.26=D:\Jenkins\Jobs\UE_4.26
set EnginePath_4.27=D:\Jenkins\Jobs\UE_4.27
set EnginePath_5.0=D:\Jenkins\Jobs\UE_5.0
set EnginePath_4.26_InstalledBuild=D:\Jenkins\Jobs\UE_4.26_InstalledBuild
set UBTRelativePathV4=Engine\Binaries\DotNet\UnrealBuildTool.exe
set UBTRelativePathV5=Engine\Binaries\DotNet\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe


rem !! Engine version for packaging !!
set RunUATPath=%EnginePath_4.27%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=D:\Jenkins\Jobs\UE_TPS
set ProjectName=JenkinsTPS.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build Params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other Params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln