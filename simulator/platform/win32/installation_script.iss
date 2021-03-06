; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "EEZ PSU Simulator"
#define MyAppVersion "0.3.0"
#define MyAppPublisher "Envox d.o.o."
#define MyAppURL "http://www.envox.hr/eez/bench-power-supply/psu-introduction.html"
#define MyAppExeName "eez_psu_sim.exe"


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{4815CDD8-3BDE-4774-895B-E37842B93367}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\LICENSE.TXT
OutputDir=C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Installation
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\eez_psu_sim.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\eez_imgui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\SDL2_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\platform\win32\Release\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\fonts\*"; DestDir: "{app}\fonts"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\simulator\images\*"; DestDir: "{app}\images"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\martin\Dropbox\Code\EEZ\psu-firmware\doc\SCPI reference guide\EEZ PSU SCPI reference guide.pdf"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\EEZ PSU SCPI reference guide.pdf"; Description: "Display SCPI reference guide"; Flags: shellexec runasoriginaluser postinstall skipifsilent
