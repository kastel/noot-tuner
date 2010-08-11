#include "mingw/version.h"

[Setup]
Compression=lzma/max
AppName=NOOT Instrument Tuner
AppVerName=NOOT Instrument Tuner {#VERSION_MAJOR}.{#VERSION_MINOR}.{#VERSION_BUILD}
DefaultDirName={pf}\NOOT Instrument Tuner
DefaultGroupName=NOOT Instrument Tuner
VersionInfoCompany=Davide Castellone
VersionInfoProductName=NOOT Instrument Tuner Setup
AlwaysShowComponentsList=False
AppId={{81979DE0-FE6C-11DD-968B-001B3876CBEE}}
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
VersionInfoVersion={#VERSION_MAJOR}.{#VERSION_MINOR}.{#VERSION_BUILD}
VersionInfoProductVersion={#VERSION_MAJOR}.{#VERSION_MINOR}.{#VERSION_BUILD}
OutputBaseFilename=noot-tuner{#VERSION_MAJOR}.{#VERSION_MINOR}.{#VERSION_BUILD}-setup
OutputDir=win-setup
UsePreviousAppDir=False

[Languages]
Name: en; MessagesFile: compiler:Default.isl; LicenseFile: Z:\home\davide\Projects\noot-tuner\COPYING
Name: it; MessagesFile: compiler:Languages\Italian.isl; LicenseFile: Z:\home\davide\Projects\noot-tuner\COPYING

[Files]
Source: Z:\home\mingw\bin\noot-tuner.exe; DestDir: {app}
Source: Z:\home\mingw\bin\libfftw3-3.dll; DestDir: {app}
Source: Z:\home\mingw\bin\libportaudio-2.dll; DestDir: {app}
Source: Z:\home\mingw\bin\mingwm10.dll; DestDir: {app}
Source: Z:\home\mingw\bin\wxbase28u_gcc_custom.dll; DestDir: {app}
Source: Z:\home\mingw\bin\wxmsw28u_core_gcc_custom.dll; DestDir: {app}
Source: Z:\home\mingw\bin\wxmsw28u_adv_gcc_custom.dll; DestDir: {app}
Source: Z:\home\mingw\bin\wxmsw28u_html_gcc_custom.dll; DestDir: {app}
Source: Z:\home\mingw\share\locale\it\LC_MESSAGES\noot-tuner.mo; DestDir: {app}\it; Languages: it
Source: Z:\home\mingw\share\noot-tuner\noot-tuner.htb; DestDir: {app}
Source: Z:\home\mingw\share\noot-tuner\it\noot-tuner.htb; DestDir: {app}\it; Languages: it

[Dirs]
Name: {app}
Name: {app}\it; Languages: it
[Icons]
Name: {group}\NOOT Instrument Tuner; Filename: {app}\noot-tuner.exe
