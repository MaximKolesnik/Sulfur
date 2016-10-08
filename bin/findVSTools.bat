@ECHO off

SET SFVSToolsPath=""
SET TempToolsPath=""

FOR /f "tokens=2,*" %%A IN ('REG.exe query HKLM\Software\Wow6432Node\Microsoft\VisualStudio\14.0 /v "InstallDir"') DO (
	SET TempToolsPath="%%B\..\..\VC"
)


IF NOT %TempToolsPath% == "" (
	CALL :normalise %TempToolsPath%
	EXIT /B 0
)
GOTO :ERROR

:normalise
SET SFVSToolsPath=%~f1
@ECHO %SFVSToolsPath%
GOTO :EOF

:ERROR
@ECHO Visual Studio 2015 not found
EXIT /B 1