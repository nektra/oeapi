NSISDIR="C:\Program Files\NSIS"
MAKENSIS=$(NSISDIR)\makensis.exe
DOXYGEN="c:\program files\doxygen\bin\doxygen.exe"

BUILDS=debug evaluation standard

all: clean-all build-all installer-all 

clean-all: clean-debug clean-evaluation clean-standard clean-doc clean-installer

build-all: build-debug build-evaluation build-standard build-enterprise

installer-all: installer-standard installer-evaluation installer-enterprise installer-enterprise-source


build-common:
	devenv oeapi.sln /Build "$(BUILDTYPE)|Win32"
	devenv demo/demos.sln /Build "Release|Win32"

build-debug:
	set BUILDTYPE=Debug
	$(MAKE) build-common 

build-evaluation:
	set BUILDTYPE=Evaluation
	$(MAKE) build-common

build-enterprise:
	set BUILDTYPE=Enterprise
	$(MAKE) build-common

build-standard:
	set BUILDTYPE=Standard
	$(MAKE) build-common

clean-common:
	devenv oeapi.sln /Clean "$(BUILDTYPE)|Win32"
	devenv demo/demos.sln /Clean "Release|Win32"	

clean-debug:
	set BUILDTYPE=Debug
	$(MAKE) clean-common 

clean-evaluation:
	set BUILDTYPE=Evaluation
	$(MAKE) clean-common

clean-standard:
	set BUILDTYPE=Standard
	$(MAKE) clean-common

clean-enterprise:
	set BUILDTYPE=Enterprise
	$(MAKE) clean-common

installer-common: doc 
	echo !define WLMAILAPI_BUILDTYPE_STR "$(BUILDTYPE)" > installer\buildtype.nsh 

	tools\remove_eval_build.py oeapiinitcom\oeapiinitcom.vcproj > oeapiinitcom\oeapiinitcom-distro.vcproj
	tools\remove_eval_build.py oestore\oestore.vcproj > oestore\oestore-distro.vcproj
	tools\remove_eval_build.py oecom\oecom.vcproj > oecom\oecom-distro.vcproj
	tools\remove_eval_build.py plugin\oeapiplugin.vcproj > plugin\oeapiplugin-distro.vcproj

	subwcrev . installer\version.nsh.in installer\version.nsh
	subwcrev . tools\buildversion.py.in tools\buildversion.py
	


installer-evaluation: build-evaluation installer-common
	$(MAKENSIS) installer\evaluation.nsi	
	tools\symbolzip.py .\Evaluation .\installer oeapi evaluation

installer-standard: build-standard installer-common
	$(MAKENSIS) installer\standard.nsi
	tools\symbolzip.py .\Standard .\installer oeapi standard

installer-enterprise: build-enterprise installer-common
	$(MAKENSIS) installer\enterprise.nsi
	tools\symbolzip.py .\Enterprise .\installer oeapi enterprise

installer-enterprise-source: build-enterprise installer-common
	$(MAKENSIS) installer\enterprise-source.nsi
	tools\symbolzip.py .\Enterprise .\installer oeapi enterprise-src

clean-installer:
	del installer\buildtype.nsh
	del installer\version.nsh

	del oeapiinitcom\oeapiinitcom-distro.vcproj
	del oestore\oestore-distro.vcproj
	del oecom\oecom-distro.vcproj
	del plugin\oeapiplugin-distro.vcproj

doc:
	cd Documentation
	$(DOXYGEN)
	cd ..

clean-doc:
	del Documentation\OEAPI-Help.chm
	del Documentation\html /S /Q

