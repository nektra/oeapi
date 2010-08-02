!insertmacro GetParent
!insertmacro DirState

;SetPluginUnload alwaysoff
Function .onInit
  Push $R0
  Push $R1
  Push $R2
  Push $R3

;; Check nsis installer key
  ReadRegStr $R0 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "UninstallString"
  StrCmp $R0 "" check_msi

  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${PRODUCT_NAME} is already installed. $\n$\nClick 'OK' to remove the \
  previous version or 'Cancel' to cancel this upgrade." IDOK remove_nsis
  Abort

;Run the uninstaller
remove_nsis:
  ;ReadRegStr $R1 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "ProductPath"
  ${GetParent} "$R0" $R1
  ReadRegStr $R2 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "DemoPath"

  WriteRegStr "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "OeapiUpgrade" "True"

  ClearErrors
  ; Copy the uninstaller to a temp location
  GetTempFileName $0
  CopyFiles $R0 $0
  ;Start the uninstaller using the option to not copy itself
  ExecWait '"$0" _?=$INSTDIR' $R3

  IntCmp $R3 128 query_reboot
  IntCmp $R3 0 check_folders
  
  goto remove_error

check_folders:
; Check if the ProgramFiles\OEAPI is empty
  ${DirState} "$R1" $R3
  IntCmp $R3 -1 check_demo_path

; Check if demo folder is empty
check_demo_path:
  StrCmp $R2 "" continue_upgrade
  ${DirState} "$R2" $R3
  IntCmp $R3 -1 continue_upgrade

query_reboot:
; Not empty reboot required
  MessageBox MB_YESNO|MB_ICONEXCLAMATION \
  "In order to complete the uninstall of the previous version$\n\
  a reboot is required.$\n$\n\
  Do you want to reboot now?" IDYES reboot_before
  Abort
  
reboot_before:
  ;MessageBox MB_OK "<$CMDLINE>"
  WriteRegStr "HKLM" "Software\Microsoft\Windows\CurrentVersion\RunOnce" "Nektra OEAPI Install" "$CMDLINE"
  
  Reboot
  goto done

remove_error:
  MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to remove previous version of ${PRODUCT_NAME}"
  Abort

  ; remove the copied uninstaller
  Delete '$0'

  goto done

check_msi:
  ReadRegStr $R0 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY_MSI}" "UninstallString"
  StrCmp $R0 "" done

  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "A previous version of ${PRODUCT_NAME} was found. \
  It is recommended that you uninstall it first.$\n$\n\
  Do you want to do that now?" IDOK uninst_msi
  Abort

uninst_msi:
  ExecWait '"msiexec.exe" /x ${PRODUCT_CODE}'

continue_upgrade:
  MessageBox MB_OK|MB_ICONINFORMATION \
      "Click OK to continue upgrading your version of ${PRODUCT_NAME}"

done:
  Pop $R3
  Pop $R2
  Pop $R1
  Pop $R0
FunctionEnd


Section "SymNektra OEAPI" Binaries
  SectionIn 1 RO
  ;; SetAutoClose false
  ;; SetAutoClose true
  SetOverwrite on
  SetOutPath "$INSTDIR\Bin"
  File "..\${PRODUCT_BINARIES_DIR}\Launcher.exe"
  File "..\${PRODUCT_BINARIES_DIR}\oecom.dll"
  File "..\${PRODUCT_BINARIES_DIR}\oeapiinitcom.dll" 
  File "..\${PRODUCT_BINARIES_DIR}\oestore.dll"
  File "..\${PRODUCT_BINARIES_DIR}\oehook.dll"
  
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "SymNektra OEAPI" "$INSTDIR\Bin\Launcher.exe"
SectionEnd


Section "Documentation" Documentation
  SetOutPath "$INSTDIR\Doc"
  File "..\doc\Install.rtf"
  File "..\doc\OEAPI Dependencies.rtf"
  File "..\doc\oeapi-help.chm"
  File "..\doc\Readme.rtf"

  File "..\doc\compiling-oeapi.txt"

;  SetOutPath "$INSTDIR\Doc\Html"
;  File "..\doc\html\*.html"
;  File "..\doc\html\*.css"
;  File "..\doc\html\*.png"
;  File "..\doc\html\*.gif"
  
  
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help.lnk" "$INSTDIR\Doc\oeapi-help.chm"
  ;CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help (HTML).lnk" "$INSTDIR\Doc\Html\index.html"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Compiling OEAPI.lnk" "$INSTDIR\Doc\compiling-oeapi.txt"
  
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "IconFile" "$INSTDIR\Bin\Launcher.exe"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "IconIndex" "0"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "ConfirmFileOp" "0"
  FlushIni "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini"
  
    ; Set the new folder as a system folder
  SetFileAttributes "$SMPROGRAMS\Nektra\OEAPI" SYSTEM
  ; Set file attributes to system and hidden
  SetFileAttributes "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" HIDDEN|SYSTEM


SectionEnd

Section "Source Code" Source
  ;!include "oeapi-source.nsh"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\Source Code"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Source Code\oeapi.sln.lnk" "$INSTDIR\Source\oeapi.sln"
SectionEnd



LangString DESC_Binaries ${LANG_ENGLISH} "Nektra OEAPI objects."
LangString DESC_Documentation ${LANG_ENGLISH} "Documentation in CHM format."
LangString DESC_Source ${LANG_ENGLISH} "Nektra OEAPI Source Code."



!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Binaries} $(DESC_Binaries)
  !insertmacro MUI_DESCRIPTION_TEXT ${Documentation} $(DESC_Documentation)
  !insertmacro MUI_DESCRIPTION_TEXT ${Source} $(DESC_Source)
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Section -Post
  ; Load Launcher if not loaded
  Exec "$INSTDIR\Bin\Launcher.exe"

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" 1
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" 1
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "ProductPath" "$INSTDIR"
SectionEnd
