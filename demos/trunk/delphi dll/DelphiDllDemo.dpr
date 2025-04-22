library DelphiDllDemo;

uses
  ComServ,
  DelphiDllDemo_TLB in 'DelphiDllDemo_TLB.pas',
  Main in 'Main.pas' {DelphiDllDemoClass: CoClass},
  OEAPIINITCOM_TLB in 'c:\program files\borland\bds\4.0\Imports\OEAPIINITCOM_TLB.pas',
  OEAPI_TLB in 'c:\program files\borland\bds\4.0\Imports\OEAPI_TLB.pas';

exports
  DllGetClassObject,
  DllCanUnloadNow,
  DllRegisterServer,
  DllUnregisterServer;

{$R *.TLB}

{$R *.RES}

begin
end.
