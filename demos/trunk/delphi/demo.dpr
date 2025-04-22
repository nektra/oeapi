program demo;

uses
  Forms,
  main in 'main.pas' {Folders},
  OEAPIINITCOM_TLB in '..\..\..\..\..\..\Program Files\Borland\Delphi7\Imports\OEAPIINITCOM_TLB.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFolders, Folders);
  Application.Run;
end.
