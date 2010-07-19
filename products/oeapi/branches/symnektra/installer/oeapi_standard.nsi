!include "oeapi-version.nsh"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "OEAPI Standard Version"
!define PRODUCT_VERSION ${OEAPI_VERSION_STR}
!define PRODUCT_PUBLISHER "Nektra S.A."
!define PRODUCT_NAME_UNINST "Nektra OEAPI"
!define PRODUCT_CODE "{587BDE76-2FDF-403D-A745-7A275084FAEB}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME_UNINST}"
!define PRODUCT_UNINST_KEY_MSI "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_CODE}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_LICENSE "license-freeware.txt"
!define PRODUCT_OUTPUT_FILE "oeapi_standard.exe"
!define PRODUCT_BINARIES_DIR "standard"

!include "oeapi-prelude.nsh"

!include "oeapi-install.nsh"

!include "oeapi-uninstall.nsh"
