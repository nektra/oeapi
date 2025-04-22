!include LogicLib.nsh
!include "include\ProcFunc.nsh"

!insertmacro un.DirState
;!insertmacro un.GetParameters

Var OeapiUpgrading

Function un.onInit
  Push $R0
  Push $R1
  
  ReadRegStr $R0 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "OeapiUpgrade"
  
  StrCpy $OeapiUpgrading $R0

  StrCmp $OeapiUpgrading "True" silent_uninstall

  IfSilent silent_uninstall
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort

  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "Please close any instance of Windows Live Mail and the WLMailApi Demo if they are running." IDOK uninstall_wlmail_closed
  Abort

uninstall_wlmail_closed:


silent_uninstall:
  Push $R1
  Push $R0
FunctionEnd

Function un.onUninstSuccess
  Push $R0
  Push $R1
  
  StrCmp $OeapiUpgrading "True" silent_uninstall
  
  IfSilent silent_uninstall
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
  
silent_uninstall:
  Push $R1
  Push $R0
FunctionEnd


Section Uninstall
  Push $R0
  Push $R1
  Push $R2
  
  ;;SetAutoClose false
  SetShellVarContext all

  InitPluginsDir
;;SetOutPath "$TEMP\oeapiuninst"

${CloseProcess} "launcher.exe" $R0

${If} $R0 > 0
  ${ProcessWaitClose} "launcher.exe" 500 $R0
${EndIf}
${If} $R0 < 0
  ${TerminateProcess} "launcher.exe" $R0
${EndIf}

DeleteRegKey HKCU "Software\\Nektra\\WLMailApi\\Settings"  
  
  ;; Remove Binaries
;;  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\WLMailApiCommon.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oehook.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oestore.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oeapiinitcom.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oecom.dll"
  Delete "$INSTDIR\Bin\launcher..exe"
  
  RmDir /r /rebootok "$INSTDIR\Bin"

  ;; Remove menu programs
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Uninstall.lnk"
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\csharpdemo.exe.lnk"
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\csharpdemo.sln.lnk"
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Run C# Dll Demo.lnk"
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Open C# Dll Demo Source.lnk"

;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Run C++ Dll Demo.lnk"
;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Open C++ Dll Demo Source.lnk"

;  Delete "$SMPROGRAMS\Nektra\WLMailApi\Desktop.ini"
  RmDir /r "$SMPROGRAMS\Nektra\WLMailApi"
  ;; Remove only if it is empty
  RmDir "$SMPROGRAMS\Nektra"
  
  DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Nektra OEApi"

  ;; Remove uninstaller
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  
  Delete "$INSTDIR\uninst.exe"
  RmDir /r /rebootok "$INSTDIR"

  ;; SetAutoClose false
  IfRebootFlag 0 noreboot
  SetErrorLevel 128
  
noreboot:
  Pop $R2
  Pop $R1
  Pop $R0
SectionEnd
