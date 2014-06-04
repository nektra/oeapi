  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\Nektra\OEApi"
  WriteIniStr "$SMPROGRAMS\Nektra\OEApi\Desktop.ini" ".ShellClassInfo" "IconFile" "$INSTDIR\Bin\oeapi.ico"
  WriteIniStr "$SMPROGRAMS\Nektra\OEApi\Desktop.ini" ".ShellClassInfo" "IconIndex" "0"
  WriteIniStr "$SMPROGRAMS\Nektra\OEApi\Desktop.ini" ".ShellClassInfo" "ConfirmFileOp" "0"
  FlushIni "$SMPROGRAMS\Nektra\OEApi\Desktop.ini"
  ; Set the new folder as a system folder
  SetFileAttributes "$SMPROGRAMS\Nektra\OEApi" SYSTEM
  ; Set file attributes to system and hidden
  SetFileAttributes "$SMPROGRAMS\Nektra\OEApi\Desktop.ini" HIDDEN|SYSTEM

;  SetOutPath "$INSTDIR\Bin\C# Exe"
;  CreateShortCut "$SMPROGRAMS\Nektra\OEApi\Run C# Exe Demo.lnk" "$INSTDIR\Bin\C# Exe\csharpdemo.exe"

;  SetOutPath "$INSTDIR\Src\C# Exe"
;  CreateShortCut "$SMPROGRAMS\Nektra\OEApi\Open C# Exe Demo Source.lnk" "$INSTDIR\Src\C# Exe\OEApiDemoC#.sln"


  ; CreateShortCut "$SMPROGRAMS\Nektra\OEApi\OEAPI-Help.lnk" "$INSTDIR\Doc\OEAPI-Help.chm"


  CreateShortCut "$SMPROGRAMS\Nektra\OEApi\Uninstall.lnk" "$INSTDIR\uninst.exe"

!ifdef PRODUCT_SOURCE
  CreateDirectory "$SMPROGRAMS\Nektra\OEApi\Source"
  SetOutPath "$INSTDIR\Source"
  CreateShortCut "$SMPROGRAMS\Nektra\OEApi\Source\Open OEApi Source.lnk" "$INSTDIR\Source\oeapi.sln"
!endif

  SetOutPath "$INSTDIR"
