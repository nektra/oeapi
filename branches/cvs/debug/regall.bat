@echo ------------------
regsvr32 /s %1 oeapiinitcom.dll
regsvr32 /s %1 oecom.dll
regsvr32 /s %1 oestore.dll
pause

