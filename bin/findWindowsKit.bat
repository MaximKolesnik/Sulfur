@ECHO off

SET SFWindowsKit=""
SET TempKitPath=""

FOR /f "tokens=2,*" %%A IN ('REG.exe query "HKLM\Software\Wow6432Node\Microsoft\Windows Kits\Installed Roots" /v "KitsRoot81"') DO (
	SET TempKitPath="%%B"
)

IF NOT %TempKitPath% == "" (
	CALL :normalise %TempKitPath%
	EXIT /B 0
)

GOTO :ERROR

:normalise
SET SFWindowsKit=%~f1
@ECHO %SFWindowsKit%
GOTO :EOF

:ERROR
@ECHO Windows Kit not found
EXIT /B 1