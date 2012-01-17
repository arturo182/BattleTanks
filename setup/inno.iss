#define MyAppName "BattleTanks"
#define MyAppVersion "1.0"
#define MyAppExeName "BattleTanks.exe"

[Setup]
AppID={{C90995C9-3929-479C-AB18-58DB4AF11EDE}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=true
OutputBaseFilename=setup
SetupIconFile=.\ikona.ico
SolidCompression=true
WizardSmallImageFile=.\smallImage.bmp
WizardImageFile=.\modernImage.bmp
InternalCompressLevel=ultra
MinVersion=,5.1.2600
VersionInfoVersion=1.0.0.0
VersionInfoDescription=BattleTanks setup
FlatComponentsList=false
UsePreviousSetupType=false
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
UsePreviousGroup=false
RestartIfNeededByRun=False
OutputDir=.

[Languages]
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; Components: Gra; 

[Files]
Source: ..\bin\BattleTanks.exe; DestDir: {app}; Flags: ignoreversion; Components: Gra; 
Source: ..\bin\EdytorMap.exe; DestDir: {app}; Flags: IgnoreVersion; Components: EdytorMap; 
Source: ..\bin\*.dll; DestDir: {app}; Flags: ignoreversion; Components: "Gra EdytorMap"; 
Source: ..\bin\baza.db; DestDir: {app}; Components: "Gra EdytorMap"; Flags: IgnoreVersion; 
Source: ..\bin\*; DestDir: {app}; Components: Gra; Flags: IgnoreVersion recursesubdirs createallsubdirs; 

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Components: Gra; 
Name: "{group}\Edytor Map"; Filename: {app}\EdytorMap.exe; IconFilename: {app}\EdytorMap.exe; Components: EdytorMap; 
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}; Components: "Gra EdytorMap"; 
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon; Components: Gra; 

[Run]
Filename: "{app}\{#MyAppExeName}"; Flags: nowait postinstall skipifsilent; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Components: Gra

[Components]
Name: Gra; Description: Gra;
Name: EdytorMap; Description: "Edytor Map";
