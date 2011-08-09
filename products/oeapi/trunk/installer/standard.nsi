!include "version.nsh"
;!include "buildtype.nsh"
!define WLMAILAPI_BUILDTYPE_STR "Standard"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "OEApi"
!define PRODUCT_VERSION ${WLMAILAPI_VERSION_STR}
!define PRODUCT_PUBLISHER "Nektra S.A."
!define PRODUCT_NAME_UNINST "Nektra OEApi"
!define PRODUCT_CODE "{587BDE76-2FDF-403D-A745-7A275084FAEB}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME_UNINST}"
!define PRODUCT_UNINST_KEY_MSI "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_CODE}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_LICENSE "unlimited-license.txt"
!define PRODUCT_OUTPUT_FILE "oeapi_v${WLMAILAPI_VERSION_STR}.exe"
!define PRODUCT_BINARIES_DIR ${WLMAILAPI_BUILDTYPE_STR}
!define DEMO_BINARIES_DIR "Release"

!include "prelude.nsh"

!include "demos.nsh"

!include "install.nsh"

!include "uninstall.nsh"

