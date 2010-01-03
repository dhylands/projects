# define installer name
outFile "installer.exe"

ShowInstDetails show

InstallDir $PROGRAMFILES\Bioloid

Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles
 
# default section start
section
 
# define output path
setOutPath $INSTDIR
 
# specify file to go in output path
!include file-list.nsh
 
# define uninstaller name
writeUninstaller $INSTDIR\uninstaller.exe
 
# default section end
sectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
section "Uninstall"

# Always delete uninstaller first
delete $INSTDIR\uninstaller.exe
 
# now delete installed file
!include file-delete.nsh
 
sectionEnd
