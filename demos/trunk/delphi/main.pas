unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, OleServer, StdCtrls, OEAPI_TLB, OESTORE_TLB, OEAPIINITCOM_TLB,
  ComCtrls;

type
  TFolders = class(TForm)
    lbFolders: TListBox;
    btnToolbar: TButton;
    btnButton: TButton;
    btnRename: TButton;
    btnCreate: TButton;
    btnDelete: TButton;
    OEAPI: TOEAPIObj;
    OEFolderManager: TOEFolderManager;
    OEToolbar: TOEToolbar;
    OEButton: TOEButton;
    OEAPIInit: TOEAPIInit;
    tvFolders: TTreeView;
    procedure OnShutdownOEAPI(Sender: TObject);
    procedure OnInitOEAPI(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnToolbarClick(Sender: TObject);
    procedure btnButtonClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure btnRenameClick(Sender: TObject);

    procedure RefreshFolders();
    procedure btnCreateClick(Sender: TObject);
    procedure btnDeleteClick(Sender: TObject);
    procedure OnLastButtonClick(Sender: TObject);
    procedure OnToolbarButtonClicked(ASender: TObject; buttonId: Integer);
    procedure OnOEAPIToolbarButtonClicked(ASender: TObject; toolbarId,
      buttonId: Integer);
  private
    { Private declarations }
    appPath: WideString;
    PrevIdentity: WideString;

    procedure LoadSubFolders(parentNode: TTreeNode; folder: IOEFolder);

  public
    { Public declarations }
  end;

var
  Folders: TFolders;

implementation

{$R *.dfm}

procedure TFolders.OnInitOEAPI(Sender: TObject);
var
  oeIdentity: IOEIdentity;
  newIdentity: WideString;
begin
  // event triggered when OE is started
  Folders.Caption := 'OE Loaded';
  OEAPI.Connect;

  oeIdentity := CoOEIdentity.Create;
  oeIdentity.GetCurrentIdentity(newIdentity);

  if newIdentity <> PrevIdentity then
  begin
    PrevIdentity := newIdentity;
    OEFolderManager.Disconnect;
    OEFolderManager.Connect;

    RefreshFolders();
  end
end;

procedure TFolders.OnShutdownOEAPI(Sender: TObject);
begin
  // event triggered when OE dies
  Folders.Caption := 'OE Not Loaded';

  // disconnect all objects. Otherwise, OE will keep running
  OEAPI.Disconnect;
  OEToolbar.Disconnect;
  OEButton.Disconnect;
end;

procedure TFolders.FormCreate(Sender: TObject);
var
  oeIdentity: IOEIdentity;
begin
  { lockZorder := False; }
  // set the window as top most
  SetWindowPos(Folders.Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE);

  appPath := Extractfilepath(application.exename);
  // appPath := GetCurrentDir();

  oeIdentity := CoOEIdentity.Create;
  oeIdentity.GetCurrentIdentity(PrevIdentity);
  RefreshFolders();
end;

procedure TFolders.LoadSubFolders(parentNode: TTreeNode; folder: IOEFolder);
var
  child: IOEFolder;
  name: WideString;
  node: TTreeNode;
  id: Integer;
begin
  folder.GetFirstChild(child);
  while child <> nil do
  begin
    child.Name(name);
    child.GetID(id);
    node := tvFolders.Items.AddChild(parentNode, name);
    node.Data := pointer(id);
    LoadSubFolders(node, child);
    folder.GetNextChild(child);
  end;
end;

procedure TFolders.RefreshFolders();
var
  oeFolder: IOEFolder;
  folderName: WideString;
  folderId: Integer;
  rootNode: TTreeNode;
begin
  //
  with tvFolders.Items do
  begin
    BeginUpdate;
    Clear;
    OEFolderManager.GetFirstFolder(oeFolder);
    oeFolder.Name(folderName);
    oeFolder.GetID(folderId);
    rootNode :=  AddFirst(nil, folderName);
    { store Id so it can be recovered later quickly }
    rootNode.Data := pointer(folderId);
    LoadSubFolders(rootNode, oeFolder);
    rootNode.Expanded := True;
    EndUpdate;
  end;
end;

procedure TFolders.btnToolbarClick(Sender: TObject);
var
    myIOEToolbar: IOEToolbar;
begin
  OEAPI.CreateToolbar(myIOEToolbar);
  OEToolbar.ConnectTo(myIOEToolbar);
end;

procedure TFolders.btnButtonClick(Sender: TObject);
var
  normalImgPath: WideString;
  mouseOverImgPath: WideString;
  myIOEButton: IOEButton;
begin

  if OEToolbar = nil then
  begin
    MessageBox(Folders.Handle, 'You must create a toolbar before.', 'Button creation error', MB_OK);
  end
  else
  begin
    // we need absolute paths for images as oecom.dll is running inside OE and
    // relative paths are relative to OE current directory
    normalImgPath := appPath + '\images\img_normal.bmp';
    mouseOverImgPath := appPath + '\images\img_mouseover.bmp';
    OEToolbar.CreateButton('Nektra', normalImgPath, mouseOverImgPath, myIOEButton);
    OEButton.ConnectTo(myIOEButton);
  end;
end;

procedure TFolders.FormDestroy(Sender: TObject);
var
  OEAPIActive: Integer;
begin
  OEAPIInit.IsOEAPIActive(OEAPIActive);

  // if we don't test if oeapi is active, any call in OEAPI will start OE
  if OEAPIActive <> 0 then
  begin
    OEAPI.RemoveAllToolbars;
  end;

  OEAPI.Disconnect;
  OEToolbar.Disconnect;
  OEButton.Disconnect;
end;

procedure TFolders.btnRenameClick(Sender: TObject);
var
  folderId: Integer;
  oeFolder: IOEFolder;
  folderName: WideString;
  newName: String;
  msg: String;
  ok: Integer;
  node: TTreeNode;
begin
  node := tvFolders.Selected;
  if node = nil then
  begin
    MessageBox(Folders.Handle, 'Select a folder first', 'Rename Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  folderId := Integer(node.Data);
  OEFolderManager.GetFolder(folderId, oeFolder);
  if oeFolder = nil then
  begin
    MessageBox(Folders.Handle, 'Invalid Folder.', 'Rename Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  oeFolder.Name(folderName);
  msg := 'New name for folder ' + folderName;
  newName := InputBox('Folder rename', msg, folderName);

  if newName <> '' then
  begin
    folderName := newName;
    oeFolder.Rename(folderName, ok);
    if ok <> 0 then
    begin
      MessageBox(Folders.Handle, 'Folder renamed successfully', 'Folder rename', MB_OK or MB_ICONINFORMATION);
      // RefreshFolders();
      node.Text := newName;
    end
  end
end;

procedure TFolders.btnCreateClick(Sender: TObject);
var
  folderId: Integer;
  oeFolder: IOEFolder;
  folderName: WideString;
  newName: String;
  msg: String;
  ok: Integer;
  node: TTreeNode;
  child: TTreeNode;
begin
  node := tvFolders.Selected;
  if node = nil then
  begin
    MessageBox(Folders.Handle, 'Select a folder first', 'Create Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  folderId := Integer(node.Data);
  OEFolderManager.GetFolder(folderId, oeFolder);
  if oeFolder = nil then
  begin
    MessageBox(Folders.Handle, 'Invalid Folder.', 'Create Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  oeFolder.Name(folderName);
  msg := 'Enter new folder name, child of folder ' + folderName;
  newName := InputBox('Folder create', msg, '');

  if newName <> '' then
  begin
    folderName := newName;
    oeFolder.CreateFolder(folderName, ok);

    if ok <> 0 then
    begin
      MessageBox(Folders.Handle, 'Successful creation', 'Create Folder', MB_OK or MB_ICONINFORMATION);
      // RefreshFolders();
      child := tvFolders.Items.AddChild(node, folderName);
      child.Data := pointer(ok); { ok is id of the created folder }
      node.Expanded := True;
      child.Selected := True;
    end
  end
end;

procedure TFolders.btnDeleteClick(Sender: TObject);
var
  folderId: Integer;
  oeFolder: IOEFolder;
  ok: Integer;
  node: TTreeNode;
begin
  node := tvFolders.Selected;
  if node = nil then
  begin
    MessageBox(Folders.Handle, 'Select a folder first.', 'Delete Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  folderId := Integer(node.Data);
  OEFolderManager.GetFolder(folderId, oeFolder);
  if oeFolder = nil then
  begin
    MessageBox(Folders.Handle, 'Invalid Folder.', 'Delete Folder', MB_OK or MB_ICONEXCLAMATION);
    Exit;
  end;

  oeFolder.Delete(ok);
  if ok <> 0 then
  begin
    MessageBox(Folders.Handle, 'Successful deletion', 'Delete Folder', MB_OK or MB_ICONINFORMATION);
    //RefreshFolders();
    node.Delete;
  end
end;

procedure TFolders.OnLastButtonClick(Sender: TObject);
begin
    MessageBox(Folders.Handle, 'Button Event', 'Toolbar button clicked', MB_OK);
end;

procedure TFolders.OnToolbarButtonClicked(ASender: TObject;
  buttonId: Integer);
begin
    MessageBox(Folders.Handle, 'Toolbar Event', 'Toolbar button clicked', MB_OK);
end;

procedure TFolders.OnOEAPIToolbarButtonClicked(ASender: TObject; toolbarId,
  buttonId: Integer);
begin
    MessageBox(Folders.Handle, 'OEAPIObj Event', 'Toolbar button clicked', MB_OK);
end;

end.

