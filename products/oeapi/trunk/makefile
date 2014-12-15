NSISDIR="C:\Program Files\NSIS"
MAKENSIS=$(NSISDIR)\makensis.exe
DOXYGEN="c:\program files\doxygen\bin\doxygen.exe"

BUILDS=debug evaluation standard enterprise

all: clean-all build-all installer-all 

clean-all: clean-debug clean-evaluation clean-standard clean-doc clean-installer clean-enterprise

build-all: build-debug build-evaluation build-standard build-enterprise

installer-all: installer-evaluation installer-standard  installer-standard-source installer-enterprise


build-common:
	devenv oeapi.sln /Build "$(BUILDTYPE)|Win32"
	devenv demo\demos.sln /Build "Release|Win32"
	tools\sign\signfile.bat $(BUILDTYPE)\oeapiinitcom.dll
	tools\sign\signfile.bat $(BUILDTYPE)\oecom.dll
	tools\sign\signfile.bat $(BUILDTYPE)\oestore.dll
	tools\sign\signfile.bat Release\oehook.dll
	tools\sign\signfile.bat Release\launcher.exe


build-debug:
	set BUILDTYPE=Debug
	$(MAKE) build-common 

build-evaluation:
	set BUILDTYPE=Evaluation
	$(MAKE) build-common

build-standard:
	set BUILDTYPE=Standard
	$(MAKE) build-common

build-enterprise:
	set BUILDTYPE=Enterprise
	$(MAKE) build-common
	
clean-common:
	devenv oeapi.sln /Clean "$(BUILDTYPE)|Win32"
	devenv demo\demos.sln /Clean "$(BUILDTYPE)|Win32"
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
	for %%f in (installer\*.exe) do tools\sign\signfile.bat %%f 
	tools\symbolzip.py .\Bin .\installer oeapi evaluation

installer-standard: build-standard installer-common
	$(MAKENSIS) installer\standard.nsi
	for %%f in (installer\*.exe) do tools\sign\signfile.bat %%f 
	tools\symbolzip.py .\Bin .\installer oeapi standard

installer-standard-source: build-standard installer-common
	$(MAKENSIS) installer\standard-source.nsi
	for %%f in (installer\*.exe) do tools\sign\signfile.bat %%f 
	tools\symbolzip.py .\Bin .\installer oeapi src

installer-enterprise: build-enterprise installer-common
	$(MAKENSIS) installer\enterprise.nsi
	for %%f in (installer\*.exe) do tools\sign\signfile.bat %%f 
	tools\symbolzip.py .\Bin .\installer oeapi enterprise
	

clean-installer:
	del installer\buildtype.nsh
	del installer\version.nsh

	del source\WLMailApiInit\WLMailApiInit-distro.vcproj
	del source\WLMailStore\WLMailStore-distro.vcproj
	del source\WLMailCom\oecom-distro.vcproj
	del source\lib\oeapiplugin-distro.vcproj

doc:
	cd doc
	$(DOXYGEN)
	cd ..

clean-doc:
	del doc\WLMailApiHelp.chm 
	del doc\html /S /Q