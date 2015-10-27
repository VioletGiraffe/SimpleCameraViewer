[Setup]
AppName=Simple Camera Viewer
AppId=SimpleCameraViewer
AppVerName=Simple Camera Viewer
DefaultDirName={pf}\Simple Camera Viewer
DefaultGroupName=Simple Camera Viewer
AllowNoIcons=true
OutputDir=.
OutputBaseFilename=SimpleCameraViewer
UsePreviousAppDir=yes

SetupIconFile=..\app\resources\icon.ico
UninstallDisplayIcon={app}\SimpleCamViewer.exe

WizardImageBackColor=clWhite
ShowTasksTreeLines=yes

SolidCompression=true
LZMANumBlockThreads=4
Compression=lzma2/ultra64
LZMAUseSeparateProcess=yes
LZMABlockSize=8192

[Tasks]
Name: startup; Description: "Automatically run the program when Windows starts"; GroupDescription: "{cm:AdditionalIcons}";

[Files]

;App binaries
Source: binaries/*; DestDir: {app}; Flags: ignoreversion

;Qt binaries
Source: binaries/Qt/*; DestDir: {app}; Flags: ignoreversion recursesubdirs

;MSVC binaries
Source: binaries/msvcr/*; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\Simple Camera Viewer; Filename: {app}\SimpleCamViewer.exe;
Name: {group}\{cm:UninstallProgram,Simple Camera Viewer}; Filename: {uninstallexe}
Name: {userstartup}\Simple Camera Viewer; Filename: {app}\SimpleCamViewer.exe; Tasks: startup

[Run]
Filename: {app}\SimpleCamViewer.exe; Description: {cm:LaunchProgram,Simple Camera Viewer}; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: dirifempty; Name: "{app}"