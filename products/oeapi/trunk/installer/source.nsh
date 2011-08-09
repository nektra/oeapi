
	!define INPATH_PRODUCT_SRCDIR "..\"

Section CopySource
	
	SetOutPath "$INSTDIR\Source"

	File /oname=oeapi.sln "${INPATH_PRODUCT_SRCDIR}\oeapi-distro.sln"
	File "${INPATH_PRODUCT_SRCDIR}\comet_tlb2h.rules"

	SetOutPath "$INSTDIR\Source\launcher"
	File "${INPATH_PRODUCT_SRCDIR}\launcher\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\launcher\*.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\launcher\*.ico"
	File "${INPATH_PRODUCT_SRCDIR}\launcher\*.rc"
	
	SetOutPath "$INSTDIR\Source\oeapicommon"
	File "${INPATH_PRODUCT_SRCDIR}\oeapicommon\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\oeapicommon\*.h"
	File "${INPATH_PRODUCT_SRCDIR}\oeapicommon\*.idl"
	File "${INPATH_PRODUCT_SRCDIR}\oeapicommon\*.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\oeapicommon\*.orig"

	SetOutPath "$INSTDIR\Source\oeapiinitcom"
	File /oname=oeapiinitcom.vcproj "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\oeapiinitcom-distro.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\*.h"
	File "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\*.def"
	File "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\*.idl"
	File "${INPATH_PRODUCT_SRCDIR}\oeapiinitcom\*.rc"
	
	SetOutPath "$INSTDIR\Source\oecom"
	File /oname=oecom.vcproj "${INPATH_PRODUCT_SRCDIR}\oecom\oecom-distro.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\oecom\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\oecom\*.h"
	File "${INPATH_PRODUCT_SRCDIR}\oecom\*.def"
	File "${INPATH_PRODUCT_SRCDIR}\oecom\*.idl"
	File "${INPATH_PRODUCT_SRCDIR}\oecom\*.rc"

	SetOutPath "$INSTDIR\Source\oehook"
	File "${INPATH_PRODUCT_SRCDIR}\oehook\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\oehook\*.h"
	File "${INPATH_PRODUCT_SRCDIR}\oehook\*.rc"
	File "${INPATH_PRODUCT_SRCDIR}\oehook\*.vcproj"

	SetOutPath "$INSTDIR\Source\oestore"
	File /oname=oestore.vcproj "${INPATH_PRODUCT_SRCDIR}\oestore\oestore-distro.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\oestore\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\oestore\*.h"
	File "${INPATH_PRODUCT_SRCDIR}\oestore\*.def"
	File "${INPATH_PRODUCT_SRCDIR}\oestore\*.idl"
	File "${INPATH_PRODUCT_SRCDIR}\oestore\*.rc"
	
	SetOutPath "$INSTDIR\Source\plugin"
	File /oname=oeapiplugin.vcproj "${INPATH_PRODUCT_SRCDIR}\plugin\oeapiplugin-distro.vcproj"
	File "${INPATH_PRODUCT_SRCDIR}\plugin\*.cpp"
	File "${INPATH_PRODUCT_SRCDIR}\plugin\*.h"
	
SectionEnd