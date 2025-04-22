  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "IconFile" "$INSTDIR\Bin\Launcher.exe"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "IconIndex" "0"
  WriteIniStr "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" ".ShellClassInfo" "ConfirmFileOp" "0"
  FlushIni "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini"
  ; Set the new folder as a system folder
  SetFileAttributes "$SMPROGRAMS\Nektra\OEAPI" SYSTEM
  ; Set file attributes to system and hidden
  SetFileAttributes "$SMPROGRAMS\Nektra\OEAPI\Desktop.ini" HIDDEN|SYSTEM

  ; CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Uninstall.lnk" "$INSTDIR\uninst.exe"
  
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Demos Folder.lnk" "${OEAPI_DEMO_DIR}\"
  
  SetOutPath "${OEAPI_DEMO_DIR}\C# DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C# DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C# DLL Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\C# DLL\demo.sln"
  
  SetOutPath "${OEAPI_DEMO_DIR}\C# EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C# EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C# EXE Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\C# EXE\demo.sln"  
  
  SetOutPath "${OEAPI_DEMO_DIR}\C++ ATL DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C++ ATL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ ATL Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\C++ ATL DLL\demo.sln"    
  
  SetOutPath "${OEAPI_DEMO_DIR}\C++ Comet EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\C++ Comet EXE\demo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\C++ Comet EXE Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\C++ Comet EXE\demo.sln"
 
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\Delphi DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi DLL Demo\demo.bdsproj.lnk" "${OEAPI_DEMO_DIR}\Delphi DLL\DelphiDllDemo.bdsproj"
  
  SetOutPath "${OEAPI_DEMO_DIR}\Delphi EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\Delphi EXE\demo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Delphi EXE Demo\demo.dpr.lnk" "${OEAPI_DEMO_DIR}\Delphi EXE\demo.dpr"
  
  SetOutPath "${OEAPI_DEMO_DIR}\VB EXE"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo\demo.exe.lnk" "${OEAPI_DEMO_DIR}\VB EXE\demo.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB EXE Demo\demo.vbp.lnk" "${OEAPI_DEMO_DIR}\VB EXE\demo.vbp"
  
  SetOutPath "${OEAPI_DEMO_DIR}\VB NET DLL"
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI\VB .NET DLL Demo"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\VB .NET DLL Demo\demo.sln.lnk" "${OEAPI_DEMO_DIR}\VB NET DLL\demo.sln"
  
  
