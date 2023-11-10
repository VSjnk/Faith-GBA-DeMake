All documentation, including this introduction, is totally incomplete
at the moment.  

Basic ReadMe for Windows:

The shortcut works only in windows NT, 2k, and XP.

For windows 9x/Me you need to run rc.bat from the command line to setup
the environment.

Make sure the value of the environment variable DEVKITADV set in the
file rc.bat matches where you installed the DevKit.  Many of the
scripts and makefiles depend on it being correct.

Edit user.bat instead of cmd-rc.bat to add new commands to DevKit command
prompt startup.  This way, when you upgrade to a new DevKit, you can
easily copy those custom commands over.

