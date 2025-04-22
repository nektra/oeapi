!include "oeapi-version.nsh"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "SymNektra OEAPI"
!define PRODUCT_VERSION ${OEAPI_VERSION_STR}
!define PRODUCT_PUBLISHER "Nektra S.A."
!define PRODUCT_NAME_UNINST "SymNektra OEAPI"
!define PRODUCT_CODE "{50966A99-B15D-4b72-906E-1D996A871ECD}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME_UNINST}"
!define PRODUCT_UNINST_KEY_MSI "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_CODE}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_LICENSE "symnektra-license.txt"
!define PRODUCT_OUTPUT_FILE "oeapi_symnektra-${PRODUCT_VERSION}.exe"
!define PRODUCT_BINARIES_DIR "release"
!define PRODUCT_INCLUDE_SOURCES 1

!include "oeapi-prelude.nsh"

!include "oeapi-install.nsh"

!include "oeapi-uninstall.nsh"
