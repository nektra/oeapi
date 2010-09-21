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
  File "..\doc\html\*.html"
  File "..\doc\html\*.css"
  File "..\doc\html\*.png"
  File "..\doc\html\*.gif"

