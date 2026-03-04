REM
REM Keenan's Hack to remove intermediate files
REM Designed to work for C++ or C# visual studio projects
REM
REM If you find a way to enhance, 
REM please return updates to ekeenan2@cdm.depaul.edu
REM

REM Set the current directory as AZUL_PATH environmental var
for %%I in (.) do set CurrDirName=%%~fI
echo %CurrDirName%

SETX AZUL_PATH "%CurrDirName%"


REM cleanup root directory

    if exist ..\Azul rmdir /S /Q ..\Azul
    if exist *.user del /F /s *.user
    if exist Azul rmdir /S /Q Azul
    if exist Logs rmdir /S /Q Logs
    if exist Debug rmdir /S /Q Debug
    if exist Release rmdir /S /Q Release
    if exist x64 rmdir /S /Q x64
    if exist ipch rmdir /S /Q ipch
    if exist .vs rmdir /S /Q .vs

    if exist shaders\compiled rmdir /S /Q shaders\compiled

    if exist *.suo del /F /S /A:H *.suo
    if exist *.sdf del /F /S *.sdf
    if exist *.suo del /F /S *.suo
    if exist *.db del /F *.db
  
REM walk through subdirectories delete Release & Debug subs

for /d %%D in (*) do (
	REM echo %%~fD
	if exist %%~D\*.bin del /F %%~D\*.bin
	if exist %%~D\Debug rmdir /S /Q %%~D\Debug
	if exist %%~D\Release rmdir /S /Q %%~D\Release
	if exist %%~D\x64 rmdir /S /Q %%~D\x64
	if exist %%~D\Azul rmdir /S /Q %%~D\Azul
	REM if exist %%~D\*.user del /F /s %%~D\*.user
)