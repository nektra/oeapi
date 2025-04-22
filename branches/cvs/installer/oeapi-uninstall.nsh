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

  SetOutPath "$PLUGINSDIR"
  File "actions\Release\actions.dll"
  SetPluginUnload manual
  System::Call /NOUNLOAD "actions::ClosePrograms(i 0.) i .r0"
  System::Call "actions::DeleteRegKey(i 0.) i .r0"
  System::Free 0

  ;Delete "$INSTDIR\actions.dll"
  
  ;; Remove Binaries
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oehook.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oestore.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oeapiinitcom.dll"
  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\Bin\oecom.dll"
  Delete "$INSTDIR\Bin\launcher.exe"
  
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
  
  Delete "$R2\c#\App.ico"
  Delete "$R2\c#\AssemblyInfo.cs"
  Delete "$R2\c#\csharpdemo.csproj"
  Delete "$R2\c#\csharpdemo.sln"
  Delete "$R2\c#\FrmCreateMsg.cs"
  Delete "$R2\c#\FrmCreateMsg.resx"
  Delete "$R2\c#\FrmEditProperty.cs"
  Delete "$R2\c#\FrmEditProperty.resx"
  Delete "$R2\c#\FrmFolders.cs"
  Delete "$R2\c#\FrmFolders.resx"
  Delete "$R2\c#\FrmInput.cs"
  Delete "$R2\c#\FrmInput.resx"
  Delete "$R2\c#\FrmMain.cs"
  Delete "$R2\c#\FrmMain.resx"
  Delete "$R2\c#\FrmMessageAttachments.cs"
  Delete "$R2\c#\FrmMessageAttachments.resx"
  Delete "$R2\c#\FrmMsgBodies.cs"
  Delete "$R2\c#\FrmMsgBodies.resx"
  RmDir "$R2\c#"

; Delphi demo
  Delete "$R2\delphi\demo.dpr"
  Delete "$R2\delphi\demo.exe"
  Delete "$R2\delphi\demo.res"
  Delete "$R2\delphi\main.dcu"
  Delete "$R2\delphi\main.dfm"
  Delete "$R2\delphi\main.pas"
  Delete "$R2\delphi\oeapi.dpk"
  Delete "$R2\delphi\oeapi.res"
  RmDir "$R2\delphi"

  Delete "$R2\images\img_mouseover.bmp"
  Delete "$R2\images\img_normal.bmp"
  RmDir "$R2\images"

; C++ Atl demo
  Delete "$R2\toolbar\images\img_mouseover.bmp"
  Delete "$R2\toolbar\images\img_normal.bmp"
  RmDir "$R2\toolbar\images"

  Delete "$R2\toolbar\atl_addin.cpp"
  Delete "$R2\toolbar\atl_addin.h"
  Delete "$R2\toolbar\atl_addin.rgs"
  Delete "$R2\toolbar\oeapi_sample.cpp"
  Delete "$R2\toolbar\oeapi_sample.def"
  Delete "$R2\toolbar\oeapi_sample.h"
  Delete "$R2\toolbar\oeapi_sample.idl"
  Delete "$R2\toolbar\oeapi_sample.rc"
  Delete "$R2\toolbar\oeapi_sample.rgs"
  Delete "$R2\toolbar\oeapi_sample.sln"
  Delete "$R2\toolbar\oeapi_sample.vcproj"
  Delete "$R2\toolbar\oeapi_sample_i.c"
  Delete "$R2\toolbar\oeapi_sample_p.c"
  Delete "$R2\toolbar\oeapi_sampleps.def"
  Delete "$R2\toolbar\oeapi_samplePS.vcproj"
  Delete "$R2\toolbar\ReadMe.txt"
  Delete "$R2\toolbar\resource.h"
  Delete "$R2\toolbar\stdafx.cpp"
  Delete "$R2\toolbar\stdafx.h"
  RmDir "$R2\toolbar"

; VB6 demo
  Delete "$R2\vb\images\img_mouseover.bmp"
  Delete "$R2\vb\images\img_normal.bmp"
  RmDir "$R2\vb\images"

  Delete "$R2\vb\demo.exe"
  Delete "$R2\vb\demo.vbp"
  Delete "$R2\vb\demo.vbw"
  Delete "$R2\vb\folderSelect.frm"
  Delete "$R2\vb\folderSelect.frm"
  Delete "$R2\vb\folderSelect.frx"
  Delete "$R2\vb\frmCreateMsg.frm"
  Delete "$R2\vb\frmCreateMsg.frx"
  Delete "$R2\vb\frmMessageAttachs.frm"
  Delete "$R2\vb\frmMessageAttachs.frx"
  Delete "$R2\vb\frmMessageOps.frm"
  Delete "$R2\vb\frmMessageOps.frx"
  Delete "$R2\vb\frmMessageProps.frm"
  Delete "$R2\vb\frmMessageProps.frx"
  Delete "$R2\vb\frmMsg.frm"
  Delete "$R2\vb\frmMsg.frx"
  Delete "$R2\vb\frmMsgWnds.frm"
  Delete "$R2\vb\frmMsgWnds.frx"
  Delete "$R2\vb\frmToolbarProps.frm"
  Delete "$R2\vb\frmToolbarProps.frx"
  Delete "$R2\vb\Message.frm"
  Delete "$R2\vb\Message.frx"
  Delete "$R2\vb\testcom.frm"
  Delete "$R2\vb\testcom.frx"
  RmDir "$R2\vb"

; VB .NET demo
  Delete "$R2\vbnet\accept.bmp"
  Delete "$R2\vbnet\AssemblyInfo.vb"
  Delete "$R2\vbnet\cog.bmp"
  Delete "$R2\vbnet\delete.bmp"
  Delete "$R2\vbnet\OeapiPlugin.vb"
  Delete "$R2\vbnet\OeapiTools.vb"
  Delete "$R2\vbnet\OeapiVBNetDemo.sln"
  Delete "$R2\vbnet\OeapiVBNetDemo.vbproj"
  Delete "$R2\vbnet\wrench_orange.bmp"
  RmDir "$R2\vbnet"

; MFC VC demo
  Delete "$R2\vc\images\img_mouseover.bmp"
  Delete "$R2\vc\images\img_normal.bmp"
  RmDir "$R2\vc\images"

  Delete "$R2\vc\res\demo.rc2"
  Delete "$R2\vc\res\eapi.ico"
  RmDir "$R2\vc\res"

  Delete "$R2\vc\demo.cpp"
  Delete "$R2\vc\demo.dsp"
  Delete "$R2\vc\Release\demo.exe"
  Delete "$R2\vc\demo.h"
  Delete "$R2\vc\demo.rc"
  Delete "$R2\vc\demoDlg.cpp"
  Delete "$R2\vc\demoDlg.h"
  Delete "$R2\vc\FolderSelect.cpp"
  Delete "$R2\vc\FolderSelect.h"
  Delete "$R2\vc\InputDlg.cpp"
  Delete "$R2\vc\InputDlg.h"
  Delete "$R2\vc\MessagesDlg.cpp"
  Delete "$R2\vc\MessagesDlg.h"
  Delete "$R2\vc\MsgDlg.cpp"
  Delete "$R2\vc\MsgDlg.h"
  Delete "$R2\vc\OEAPI.h"
  Delete "$R2\vc\OEAPIINITCOM.h"
  Delete "$R2\vc\oeapi_wrapper.cpp"
  Delete "$R2\vc\oeapi_wrapper.h"
  Delete "$R2\vc\OESTORE.h"
  Delete "$R2\vc\resource.h"
  Delete "$R2\vc\StdAfx.cpp"
  Delete "$R2\vc\StdAfx.h"
  Delete "$R2\vc\utils.cpp"
  Delete "$R2\vc\utils.h"
  RmDir "$R2\vc"

  RMDir /r /rebootok "$R2"
  
no_demo_files:

!ifdef PRODUCT_INCLUDE_SOURCES
  RmDir /r /rebootok "$INSTDIR\src"
!endif

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
