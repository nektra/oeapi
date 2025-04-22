; Given a .NET version number, this function returns that .NET framework's
; install directory. Returns "" if the given .NET version is not installed.
; Params: [version] (eg. "v2.0")
; Return: [dir] (eg. "C:\WINNT\Microsoft.NET\Framework\v2.0.50727")

Function GetDotNetDir
	Exch $R0 ; Set R0 to .net version major
	Push $R1
	Push $R2
 
	; set R1 to minor version number of the installed .NET runtime
	EnumRegValue $R1 HKLM \
		"Software\Microsoft\.NetFramework\policy\$R0" 0
	IfErrors getdotnetdir_err
 
	; set R2 to .NET install dir root
	ReadRegStr $R2 HKLM \
		"Software\Microsoft\.NetFramework" "InstallRoot"
	IfErrors getdotnetdir_err
 
	; set R0 to the .NET install dir full
	StrCpy $R0 "$R2$R0.$R1"
 
getdotnetdir_end:
	Pop $R2
	Pop $R1
	Exch $R0 ; return .net install dir full
	Return
 
getdotnetdir_err:
	StrCpy $R0 ""
	Goto getdotnetdir_end
 
FunctionEnd