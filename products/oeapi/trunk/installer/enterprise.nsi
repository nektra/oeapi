!define PRODUCT_NAME "OEAPI Enterprise Version"
!define PRODUCT_VERSION ${OEAPI_VERSION_STR}
!define PRODUCT_LICENSE "unlimited-license.txt"
!define PRODUCT_OUTPUT_FILE "oeapi_v${OEAPI_VERSION_STR}_enterprise.exe"

!define BUILDTYPE "Enterprise"
!define RELEASE_BINARIES_DIR "..\Enterprise"
!define COMMON_BINARIES_DIR "..\Release"

!include "prelude.nsh"
!include "binaries.nsh"
!include "documentation.nsh"
!include "demos.nsh"

LangString DESC_Binaries ${LANG_ENGLISH} "Nektra OEAPI COM objects."
LangString DESC_Documentation ${LANG_ENGLISH} "Documentation in CHM and HTML formats."
LangString DESC_Samples ${LANG_ENGLISH} "Sample projects in VB, C#, C++ and Delphi."

!ifdef PRODUCT_INCLUDE_SOURCES
LangString DESC_Source ${LANG_ENGLISH} "Nektra OEAPI Source Code."
!endif


!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Binaries} $(DESC_Binaries)
  !insertmacro MUI_DESCRIPTION_TEXT ${Documentation} $(DESC_Documentation)
  !insertmacro MUI_DESCRIPTION_TEXT ${Samples} $(DESC_Samples)
!ifdef PRODUCT_INCLUDE_SOURCES
  !insertmacro MUI_DESCRIPTION_TEXT ${Source} $(DESC_Source)
!endif
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Section -Post
  ; Load Launcher if not loaded
  Exec "$INSTDIR\Bin\Launcher.exe"

  WriteUninstaller "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\Nektra\OEAPI\Uninstall.lnk" "$INSTDIR\uninst.exe"
	
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" 1
  WriteRegDword ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" 1
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "ProductPath" "$INSTDIR"
SectionEnd


!include "uninstall.nsh"
