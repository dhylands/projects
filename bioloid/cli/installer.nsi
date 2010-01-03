!include EnvVarUpdate.nsh

# define installer name
outFile "installer.exe"

ShowInstDetails show
ShowUnInstDetails show
InstallDir $PROGRAMFILES\Bioloid

Name "Bioloid Command Line Utility"

Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles
 
# default section start
Section "Bioloid Command Line Utility"

# define output path
setOutPath $INSTDIR
 
# specify file to go in output path
!include file-list.nsh
 
# define uninstaller name
writeUninstaller $INSTDIR\uninstaller.exe

${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR"
 
# default section end
sectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
section "Uninstall"

# Always delete uninstaller first
delete $INSTDIR\uninstaller.exe
 
# now delete installed file
!include file-delete.nsh

${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR"
 
sectionEnd
