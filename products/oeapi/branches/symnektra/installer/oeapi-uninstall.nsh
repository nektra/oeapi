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
  Push $R1
  Push $R2
  
  ;;SetAutoClose false
  SetShellVarContext all

  InitPluginsDir
;;SetOutPath "$TEMP\oeapiuninst"

  ;SetOutPath "$PLUGINSDIR"
  ;File "actions\Release\actions.dll"
  ;SetPluginUnload manual
  ;System::Call /NOUNLOAD "actions::ClosePrograms(i 0.) i .r0"
  ;System::Call "actions::DeleteRegKey(i 0.) i .r0"
  ;System::Free 0

  ;Delete "$INSTDIR\actions.dll"
  
  ;; Remove Binaries
 
  RmDir /r /rebootok "$INSTDIR\Bin"

  ;; Remove doc
  ${un.DirState} "$INSTDIR\Doc" $R1
  IntCmp $R1 -1 no_doc_files

  Delete "$INSTDIR\Doc\Html\*.html"
  Delete "$INSTDIR\Doc\Html\*.css"
  Delete "$INSTDIR\Doc\Html\*.png"
  Delete "$INSTDIR\Doc\Html\*.gif"

  Delete "$INSTDIR\Doc\Install.rtf"
  Delete "$INSTDIR\Doc\OEAPI Dependencies.rtf"
  Delete "$INSTDIR\Doc\oeapi-help.chm"
  Delete "$INSTDIR\Doc\Readme.rtf"
  Delete "$INSTDIR\Doc\oeapiinitcom.idl"
  Delete "$INSTDIR\Doc\oecom.idl"
  Delete "$INSTDIR\Doc\oestore.idl"

  RmDir /rebootok "$INSTDIR\Doc\Html"
  
  ;;${un.DirState} "$INSTDIR\Doc" $R1
  RmDir /r /rebootok "$INSTDIR\Doc"

no_doc_files:
  
  ;; Remove demos
  ReadRegStr $R2 "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "DemoPath"
  StrCmp $R2 "" no_demo_files
  
  ${un.DirState} "$R2" $R1
  IntCmp $R1 -1 no_demo_files

  Delete "$R2\images\img_mouseover.bmp"
  Delete "$R2\images\img_normal.bmp"
  RmDir "$R2\images"



  RMDir /r /rebootok "$R2"
  
no_demo_files:

  RmDir /r /rebootok "$INSTDIR\src"

  ;; Remove menu programs

  Delete "$SMPROGRAMS\Nektra\OEAPI\Uninstall.lnk"
  Delete "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help.lnk"
  Delete "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help (HTML).lnk"

  Delete "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini"
  RmDir /r "$SMPROGRAMS\Nektra\OEAPI"
  ;; Remove only if it is empty
  RmDir "$SMPROGRAMS\Nektra"
  
  DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Nektra OEAPI" 

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
SectionEnd
