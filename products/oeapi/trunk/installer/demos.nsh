!include "include\GetDotNetDir.nsh"

;!define OEAPI_DEMO_DIR "$DOCUMENTS\Nektra OEAPI Samples"
!define OEAPI_DEMO_DIR "$INSTDIR\Demos"


Section "Sample Projects" Samples
    
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI"
  ; Set the new folder as a system folder
  SetFileAttributes "$SMPROGRAMS\Nektra\OEAPI" SYSTEM
 
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DemosPath" "${OEAPI_DEMO_DIR}"
  
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Demos Folder.lnk" "${OEAPI_DEMO_DIR}\"
    
; C# DLL demo
	SetOutPath "${OEAPI_DEMO_DIR}\C# DLL"
	File "..\demo\c# dll\AssemblyInfo.cs"
	File "..\demo\c# dll\Plugin.csproj"
	File "..\demo\c# dll\img_mouseover.bmp"
	File "..\demo\c# dll\img_normal.bmp"
	File "..\demo\c# dll\Main.cs"
	File "..\demo\c# dll\OeapiTools.cs"
	File "..\demo\c# dll\Plugin.csproj"

	File "..\demo\c# dll\Release\OeapiPlugin.dll" 
	
	; get directory of .NET framework installation
	Push "v2.0"
	Call GetDotNetDir
	Pop $R0 ; .net framework v2.0 installation directory
	StrCmpS "" $R0 err_dot_net_not_found

	ExecWait '"$R0\RegAsm.exe" OeapiPlugin.dll'
 
err_dot_net_not_found:

  
  SetOutPath "${OEAPI_DEMO_DIR}\C# DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C# DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C# DLL Demo\Plugin.csproj.lnk" "${OEAPI_DEMO_DIR}\C# DLL\Plugin.csproj"  
  
; C# EXE demo
  SetOutPath "${OEAPI_DEMO_DIR}\C# EXE"
  File "..\demo\c#\oeapi.ico"
  File "..\demo\c#\AssemblyInfo.cs"
  File "..\demo\c#\csdemo.csproj"
  File "..\demo\c#\FrmCreateMsg.cs"
  File "..\demo\c#\FrmCreateMsg.resx"
  File "..\demo\c#\FrmEditProperty.cs"
  File "..\demo\c#\FrmEditProperty.resx"
  File "..\demo\c#\FrmFolders.cs"
  File "..\demo\c#\FrmFolders.resx"
  File "..\demo\c#\FrmInput.cs"
  File "..\demo\c#\FrmInput.resx"
  File "..\demo\c#\FrmMain.cs"
  File "..\demo\c#\FrmMain.resx"
  File "..\demo\c#\FrmMessageAttachments.cs"
  File "..\demo\c#\FrmMessageAttachments.resx"
  File "..\demo\c#\FrmMsgBodies.cs"
  File "..\demo\c#\FrmMsgBodies.resx"
  File "..\demo\c#\Release\csharpdemo.exe"
  
  SetOutPath "${OEAPI_DEMO_DIR}\C# EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C# EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C# EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\C# EXE\csharpdemo.exe"  
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C# EXE Demo\csdemo.csproj.lnk" "${OEAPI_DEMO_DIR}\C# EXE\csdemo.csproj"    

; C++ Atl demo
  SetOutPath "${OEAPI_DEMO_DIR}\C++ ATL DLL"
  File "..\demo\toolbar\atl_addin.cpp"
  File "..\demo\toolbar\atl_addin.h"
  File "..\demo\toolbar\atl_addin.rgs"
  File "..\demo\toolbar\oeapi_sample.cpp"
  File "..\demo\toolbar\oeapi_sample.def"
  File "..\demo\toolbar\oeapi_sample.idl"
  File "..\demo\toolbar\oeapi_sample.rc"
  File "..\demo\toolbar\oeapi_sample.rgs"
  File "..\demo\toolbar\oeapi_sample.vcproj"
  File "..\demo\toolbar\resource.h"
  File "..\demo\toolbar\stdafx.cpp"
  File "..\demo\toolbar\stdafx.h"

  !insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "..\demo\toolbar\Release\oeapi_sample.dll" "${OEAPI_DEMO_DIR}\C++ ATL DLL\oeapi_sample.dll" "${OEAPI_DEMO_DIR}\C++ ATL DLL"
	
  SetOutPath "${OEAPI_DEMO_DIR}\C++ ATL DLL\images"
  File "..\demo\toolbar\images\img_mouseover.bmp"
  File "..\demo\toolbar\images\img_normal.bmp"

  SetOutPath "${OEAPI_DEMO_DIR}\C++ ATL DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C++ ATL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ ATL Demo\csdemo.csproj.lnk" "${OEAPI_DEMO_DIR}\C++ ATL DLL\csdemo.csproj"      

  

; MFC VC demo
  SetOutPath "${OEAPI_DEMO_DIR}\C++ Comet EXE"
  File "..\demo\vc\common-controls-v6.manifest"
  File "..\demo\vc\demo.cpp"
  File "..\demo\vc\demo.vcproj"
  File "..\demo\vc\demo.h"
  File "..\demo\vc\demo.rc"
  File "..\demo\vc\demoDlg.cpp"
  File "..\demo\vc\demoDlg.h"
  File "..\demo\vc\FolderSelect.cpp"
  File "..\demo\vc\FolderSelect.h"
  File "..\demo\vc\InputDlg.cpp"
  File "..\demo\vc\InputDlg.h"
  File "..\demo\vc\MessagesDlg.cpp"
  File "..\demo\vc\MessagesDlg.h"
  File "..\demo\vc\MsgDlg.cpp"
  File "..\demo\vc\MsgDlg.h"
  File "..\demo\vc\oeapi_wrapper.cpp"
  File "..\demo\vc\oeapi_wrapper.h"
  File "..\demo\vc\resource.h"
  File "..\demo\vc\StdAfx.cpp"
  File "..\demo\vc\StdAfx.h"
  File "..\demo\vc\utils.cpp"
  File "..\demo\vc\utils.h"

  File "..\oecom\OEAPI.h"
  File "..\oeapiinitcom\OEAPIINITCOM.h"
  File "..\oestore\OESTORE.h"
  
  File "..\demo\vc\Release\exedemo.exe"
  
  SetOutPath "${OEAPI_DEMO_DIR}\C++ Comet EXE\images"
  File "..\demo\vc\images\img_mouseover.bmp"
  File "..\demo\vc\images\img_normal.bmp"

  SetOutPath "${OEAPI_DEMO_DIR}\C++ Comet EXE\res"
  File "..\demo\vc\res\demo.rc2"
  File "..\demo\vc\res\oeapi.ico"

  
  SetOutPath "${OEAPI_DEMO_DIR}\C++ Comet EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo\exedemo.exe.lnk" "${OEAPI_DEMO_DIR}\C++ Comet EXE\exedemo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo\demo.vcproj.lnk" "${OEAPI_DEMO_DIR}\C++ Comet EXE\demo.vcproj"  

  
  ; VB6 demo
  SetOutPath "${OEAPI_DEMO_DIR}\VB EXE"
  ;File "..\demo\vb\demo.exe"
  File "..\demo\vb\demo.exe.manifest"
  File "..\demo\vb\demo.vbp"
  File "..\demo\vb\demo.vbw"
  File "..\demo\vb\folderSelect.frm"
  File "..\demo\vb\folderSelect.frm"
  File "..\demo\vb\folderSelect.frx"
  File "..\demo\vb\frmCreateMsg.frm"
  File "..\demo\vb\frmCreateMsg.frx"
  File "..\demo\vb\frmMessageAttachs.frm"
  File "..\demo\vb\frmMessageAttachs.frx"
  File "..\demo\vb\frmMessageOps.frm"
  File "..\demo\vb\frmMessageOps.frx"
  File "..\demo\vb\frmMessageProps.frm"
  File "..\demo\vb\frmMessageProps.frx"
  File "..\demo\vb\frmMsg.frm"
  File "..\demo\vb\frmMsg.frx"
  File "..\demo\vb\frmMsgWnds.frm"
  File "..\demo\vb\frmMsgWnds.frx"
  File "..\demo\vb\frmToolbarProps.frm"
  File "..\demo\vb\frmToolbarProps.frx"
  File "..\demo\vb\Init.bas"
  File "..\demo\vb\Message.frm"
  File "..\demo\vb\Message.frx"
  File "..\demo\vb\testcom.frm"
  File "..\demo\vb\testcom.frx"
  ;File "..\demo\vb\Readme.txt"

  SetOutPath "${OEAPI_DEMO_DIR}\VB EXE\images"
  File "..\demo\vb\images\img_mouseover.bmp"
  File "..\demo\vb\images\img_normal.bmp"

  SetOutPath "${OEAPI_DEMO_DIR}\VB EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo"
  ;;CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\VB EXE\demo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo\demo.vbp.lnk" "${OEAPI_DEMO_DIR}\VB EXE\demo.vbp"
  
; VB .NET demo
  SetOutPath "${OEAPI_DEMO_DIR}\VB NET DLL"
  File "..\demo\vbnet\accept.bmp"
  File "..\demo\vbnet\AssemblyInfo.vb"
  File "..\demo\vbnet\cog.bmp"
  File "..\demo\vbnet\delete.bmp"
  File "..\demo\vbnet\demo.sln"
  File "..\demo\vbnet\demo.vbproj"
  File "..\demo\vbnet\OeapiPlugin.vb"
  File "..\demo\vbnet\OeapiTools.vb"
  File "..\demo\vbnet\wrench_orange.bmp"
  ;File "..\demo\vbnet\Readme.txt"

  SetOutPath "${OEAPI_DEMO_DIR}\VB NET DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\VB .NET DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB .NET DLL Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\VB NET DLL\demo.sln"      
  
; Delphi DLL demo
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi DLL"
  File "..\demo\delphi dll\DelphiDllDemo.bdsproj"
  File "..\demo\delphi dll\DelphiDllDemo.dpr"
  File "..\demo\delphi dll\DelphiDllDemo.res"
  File "..\demo\delphi dll\DelphiDllDemo.tlb"
  File "..\demo\delphi dll\DelphiDllDemo_TLB.pas"
  File "..\demo\delphi dll\Main.pas"
  
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\Delphi DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi DLL Demo\demo.bdsproj.lnk" "${OEAPI_DEMO_DIR}\Delphi DLL\DelphiDllDemo.bdsproj"  

; Delphi EXE demo
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi EXE"
  File "..\demo\delphi\demo.dpr"
  File "..\demo\delphi\demo.res"
  File "..\demo\delphi\main.dfm"
  File "..\demo\delphi\main.pas"
  File "..\demo\delphi\oeapi.dpk"
  File "..\demo\delphi\oeapi.res"

  SetOutPath "${OEAPI_DEMO_DIR}\Delphi EXE\images"
  File "..\demo\images\img_mouseover.bmp"
  File "..\demo\images\img_normal.bmp"

 
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo"
  ;;CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\Delphi EXE\demo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo\demo.dpr.lnk" "${OEAPI_DEMO_DIR}\Delphi EXE\demo.dpr"
  
  
  
SectionEnd
