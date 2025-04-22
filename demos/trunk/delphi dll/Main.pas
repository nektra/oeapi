unit Main;

{$WARN SYMBOL_PLATFORM OFF}

interface

uses
  Windows, ActiveX, Classes, ComObj, DelphiDllDemo_TLB, StdVcl, OEAPIINITCOM_TLB, OEAPI_TLB;

type
  TDelphiDllDemoClass = class(TTypedComObject, IDelphiDllDemoClass)
  Init : TOEAPIInit;
  OEAPI : TOEAPIObj;
  OEToolbar : TOEToolbar;
  OEButton : TOEButton;
  public
    procedure Initialize; override;
    destructor Destroy; override;
    procedure OnInit(sender:TObject);
    procedure OnClick(sender:TObject);
    procedure OnShutdown(sender:TObject);
  end;

implementation

uses ComServ;
procedure TDelphiDllDemoClass.Initialize;
begin
  Init := TOEAPIInit.Create(nil);
  Init.Connect;
  Init.OnInitOEAPI := OnInit;
  Init.OnShutdownOEAPI := OnShutdown;
end;

procedure TDelphiDllDemoClass.OnInit(sender:TObject);
var
  myOEToolbar: IOEToolbar;
  normalImgPath: WideString;
  mouseOverImgPath: WideString;
  myOEButton: IOEButton;
begin
  OutputDebugString('***** ONINIT *****');
  OEAPI := TOEAPIObj.Create(nil);
  OEAPI.Connect;

  OEToolbar := TOEToolbar.Create(nil);

  OEAPI.CreateToolbar(myOEToolbar);
  OEToolbar.ConnectTo(myOEToolbar);

  normalImgPath := 'c:\img_normal.bmp';
  mouseOverImgPath := 'c:\img_mouseover.bmp';

  OEButton := TOEButton.Create(nil);

  OEToolbar.CreateButton('Nektra', normalImgPath, mouseOverImgPath, myOEButton);
  OEButton.ConnectTo(myOEButton);

  OEButton.OnClick := OnClick;
end;

procedure TDelphiDllDemoClass.OnClick(sender:TOBject);
begin
  OutputDebugString('***** ONCLICK *****');
  MessageBox(0,'OnClick Event','OEAPI', MB_OK);
end;

procedure TDelphiDllDemoClass.OnShutdown(sender:TObject);
begin
  OutputDebugString('***** ONSHUTDOWN *****');

  //Disconnect and set to nil every object used,
  //otherwise the email client's process won't terminate.

  OEButton.Disconnect;
  OEButton := nil;

  OEToolbar.Disconnect;
  OEToolbar := nil;

  OEAPI.Disconnect;
  OEAPI := nil;
end;

destructor TDelphiDllDemoClass.Destroy;
begin
  Init.Disconnect;
  Init := nil;
end;

initialization
  TTypedComObjectFactory.Create(ComServer, TDelphiDllDemoClass, Class_DelphiDllDemoClass,
    ciMultiInstance, tmApartment);
end.
