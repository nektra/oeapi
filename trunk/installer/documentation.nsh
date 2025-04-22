Section "Documentation" Documentation

  SetOutPath "$INSTDIR\Doc"
  File "..\doc\Install.rtf"
  File "..\doc\OEAPI Dependencies.rtf"
  File "..\doc\oeapi-help.chm"
  File "..\doc\Readme.rtf"
  ;;File "..\doc\oeapiinitcom.idl"
  ;;File "..\doc\oecom.idl"
  ;;File "..\doc\oestore.idl"
  
!ifdef PRODUCT_INCLUDE_SOURCES
  File "..\doc\compiling-oeapi.txt"
!endif

  SetOutPath "$INSTDIR\Doc\Html"
  File "..\doc\html\*.*"

  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\Nektra\OEAPI"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help.lnk" "$INSTDIR\Doc\oeapi-help.chm"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\OEAPI Help (HTML).lnk" "$INSTDIR\Doc\Html\index.html"
!ifdef PRODUCT_INCLUDE_SOURCES
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Compiling OEAPI.lnk" "$INSTDIR\Doc\compiling-oeapi.txt"
!endif
SectionEnd