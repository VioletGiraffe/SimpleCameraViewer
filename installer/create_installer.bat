REM this script must set QTDIR32 path to the root of the Qt folder. Example:
REM set QTDIR32=k:\Qt\5\5.4\msvc2013_opengl\

call set_qt_paths.bat
set VS_TOOLS_DIR=%VS120COMNTOOLS%

SETLOCAL

RMDIR /S /Q binaries\

pushd ..\
%QTDIR32%\bin\qmake.exe -tp vc -r
popd

call "%VS_TOOLS_DIR%VsDevCmd.bat" x86
msbuild ..\SimpleCamViewer.sln /t:Rebuild /p:Configuration=Release;PlatformToolset=v120_xp

xcopy /R /Y ..\bin\release\SimpleCamViewer.exe binaries\

SETLOCAL
SET PATH=%QTDIR32%\bin\
%QTDIR32%\bin\windeployqt.exe --dir binaries\Qt --force --no-translations --release --no-compiler-runtime --no-angle binaries\SimpleCamViewer.exe
ENDLOCAL

xcopy /R /Y %SystemRoot%\SysWOW64\msvcr120.dll binaries\msvcr\
xcopy /R /Y %SystemRoot%\SysWOW64\msvcp120.dll binaries\msvcr\

del binaries\Qt\opengl*.*

ENDLOCAL

"c:\Program Files (x86)\Inno Setup 5\iscc" setup.iss