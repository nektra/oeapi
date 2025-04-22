
	!define INPATH_PRODUCT_DEMODIR "..\demo"

Section CopySource
	
	SetOutPath "$INSTDIR\Demos"

	SetOutPath "$INSTDIR\Demos\add-in"
	File "${INPATH_PRODUCT_DEMODIR}\add-in\*.dsp"
	File "${INPATH_PRODUCT_DEMODIR}\add-in\*.cpp"

	SetOutPath "$INSTDIR\Demos\c#"
	File "${INPATH_PRODUCT_DEMODIR}\c#\*.resx"
	File "${INPATH_PRODUCT_DEMODIR}\c#\*.cs"
	File "${INPATH_PRODUCT_DEMODIR}\c#\*.ico"
	File "${INPATH_PRODUCT_DEMODIR}\c#\*.csproj"
	
	SetOutPath "$INSTDIR\Demos\c# dll"
	File "${INPATH_PRODUCT_DEMODIR}\c# dll\*.cs"
	File "${INPATH_PRODUCT_DEMODIR}\c# dll\*.csproj"
	File "${INPATH_PRODUCT_DEMODIR}\c# dll\*.bmp"
	
	SetOutPath "$INSTDIR\Demos\delphi"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.dpr"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.res"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.dfm"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.pas"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.dpk"
	File "${INPATH_PRODUCT_DEMODIR}\delphi\*.txt"
	
	SetOutPath "$INSTDIR\Demos\delphi dll"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.bdsproj"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.dpr"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.res"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.tlb"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.pas"
	File "${INPATH_PRODUCT_DEMODIR}\delphi dll\*.txt"
	
	SetOutPath "$INSTDIR\Demos\images"
	File "${INPATH_PRODUCT_DEMODIR}\images\*.bmp"

	SetOutPath "$INSTDIR\Demos\toolbar"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.cpp"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.h"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.def"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.idl"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.rgs"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.rc"
	File "${INPATH_PRODUCT_DEMODIR}\toolbar\*.vcproj"
	
	SetOutPath "$INSTDIR\Demos\vb"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.manifest"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.txt"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.frx"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.frm"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.bas"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.vbp"
	File "${INPATH_PRODUCT_DEMODIR}\vb\*.vbw"
	SetOutPath "$INSTDIR\Demos\vb\images"
	File "${INPATH_PRODUCT_DEMODIR}\vb\images\*.bmp"
	
	SetOutPath "$INSTDIR\Demos\vbnet"
	File "${INPATH_PRODUCT_DEMODIR}\vbnet\*.bmp"
	File "${INPATH_PRODUCT_DEMODIR}\vbnet\*.txt"
	File "${INPATH_PRODUCT_DEMODIR}\vbnet\*.vb"
	File "${INPATH_PRODUCT_DEMODIR}\vbnet\*.vbproj"

	SetOutPath "$INSTDIR\Demos\vc"
	File "${INPATH_PRODUCT_DEMODIR}\vc\*.h"
	File "${INPATH_PRODUCT_DEMODIR}\vc\*.cpp"
	File "${INPATH_PRODUCT_DEMODIR}\vc\*.rc"
	File "${INPATH_PRODUCT_DEMODIR}\vc\*.manifest"
	SetOutPath "$INSTDIR\Demos\vc\images"
	File "${INPATH_PRODUCT_DEMODIR}\vc\images\*.bmp"
	SetOutPath "$INSTDIR\Demos\vc\res"
	File "${INPATH_PRODUCT_DEMODIR}\vc\res\*.rc2"
	File "${INPATH_PRODUCT_DEMODIR}\vc\res\*.ico"
	
SectionEnd