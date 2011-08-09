!include LogicLib.nsh
!include "include\ProcFunc.nsh"

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
  ReadRegStr $R2 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "Path"

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
; Check if the ProgramFiles\Nektra\WLMailApi is empty
  ${DirState} "$R1" $R3
  IntCmp $R3 -1 check_path

; Check if folder is empty
check_path:
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
  WriteRegStr "HKLM" "Software\Microsoft\Windows\CurrentVersion\RunOnce" "Nektra WLMailApi Install" "$CMDLINE"
  
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


Section "Nektra OEAPi" Binaries

;; begin LiveMail installation check
;;  ReadRegStr $R0 HKLM "Software\Microsoft\Windows Live Mail" "InstallRoot"
;;  StrCmp $R0 "" wlmail_not_installed wlmail_installed

;;wlmail_not_installed:
;;  MessageBox MB_ICONEXCLAMATION \
;;   "Windows Live Mail is not installed, please install it first."
;;  Abort

wlmail_installed:
;;; end LiveMail installation check

;;; begin Microsoft .Net Framework 2.0 installation check
  ;;; http://support.microsoft.com/kb/315291
  ReadRegStr $R0 HKLM "Software\Microsoft\.NETFramework\policy\v2.0" "50727"
  StrCmp $R0 "" dotnet_framework_not_installed dotnet_framework_installed

dotnet_framework_not_installed:
  MessageBox MB_ICONEXCLAMATION \
    "Microsoft .Net Framework 2.0 is not installed, please install it first."
  Abort

dotnet_framework_installed:
;;; end Microsoft .Net Framework 2.0 installation check

;------------------------------------
test_wlmail_instance:
  
  ${If} ${ProcessExists} msimn.exe
      MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
             "Please close any running instance of Outlook Express." \
             IDOK test_wlmail_instance
      Abort  
  ${EndIf}

;------------------------------------

  SectionIn 1 RO
  ;; SetAutoClose false
  ;; SetAutoClose true
  SetOverwrite on
  SetOutPath "$INSTDIR\Bin"
  File "..\Release\launcher.exe"
  !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "..\Release\oehook.dll" "$INSTDIR\Bin\oehook.dll" "$INSTDIR"
  !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "..\Standard\oecom.dll" "$INSTDIR\Bin\oecom.dll" "$INSTDIR"
  !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "..\Standard\oeapiinitcom.dll" "$INSTDIR\Bin\oeapiinitcom.dll" "$INSTDIR"
  !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "..\Standard\oestore.dll" "$INSTDIR\Bin\oestore.dll" "$INSTDIR"

  !include shortcuts.nsh
;;  !include demos.nsh


  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Nektra OEApi" "$INSTDIR\Bin\launcher.exe"
  
   ;Load WLMailApiAgent if not loaded
  Exec "$INSTDIR\Bin\launcher.exe"
SectionEnd




LangString DESC_Binaries ${LANG_ENGLISH} "Nektra OEApi"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Binaries} $(DESC_Binaries)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Nektra OEAPi" Uninstaller

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" 1
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" 1
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "ProductPath" "$INSTDIR"
SectionEnd
