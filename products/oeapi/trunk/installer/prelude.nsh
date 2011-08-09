SetCompressor /SOLID lzma

VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey "LegalCopyright" "© ${PRODUCT_PUBLISHER}"
VIAddVersionKey "FileDescription" "${PRODUCT_NAME}"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"

VIProductVersion "${PRODUCT_VERSION}.0"


; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "Library.nsh"
!include "FileFunc.nsh"


; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "oeapi.ico"
!define MUI_UNICON "oeapi.ico"

!define MUI_COMPONENTSPAGE_SMALLDESC ;No value

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "${PRODUCT_LICENSE}"
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Welcome page
!insertmacro MUI_UNPAGE_WELCOME
; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES
; Finish page
;!insertmacro MUI_UNPAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------


Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_OUTPUT_FILE}"
InstallDir "$PROGRAMFILES\Nektra\${PRODUCT_NAME}"
ShowInstDetails hide ;;  show
ShowUnInstDetails hide ;; show
