rem Customize this batch file with your own commands.

rem This file is called by rc.bat.  It is provided so
rem that custom settings can be easily moved from one
rem version of the DevKit to another.


echo You may edit user.bat to customize your environment.


rem DevKit Advance puts user executables in 3 places:
rem %DEVKITADV%/bin   - Toolchain
rem %DEVKITADV%/tools - Shell Tools
rem %DEVKITADV%       - make.exe, user.bat, and cmd-rc.bat

rem Setting the path as Existing;Toochain;ShellTools;Make
rem allows local tools to override DevKit Advance's

rem Use the arm-agb-elf-* names to be explicit.

PATH=%PATH%;%DEVKITADV%\bin;%DEVKITADV%\tools;%DEVKITADV%
