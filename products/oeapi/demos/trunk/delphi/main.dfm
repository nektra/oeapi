object Folders: TFolders
  Left = 659
  Top = 194
  BorderStyle = bsDialog
  Caption = 'OE Not Loaded'
  ClientHeight = 340
  ClientWidth = 312
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object lbFolders: TListBox
    Left = 8
    Top = 8
    Width = 201
    Height = 321
    ItemHeight = 13
    TabOrder = 0
  end
  object btnToolbar: TButton
    Left = 224
    Top = 8
    Width = 81
    Height = 25
    Caption = 'Add Toolbar'
    TabOrder = 1
    OnClick = btnToolbarClick
  end
  object btnButton: TButton
    Left = 224
    Top = 40
    Width = 81
    Height = 25
    Caption = 'Add Button'
    TabOrder = 2
    OnClick = btnButtonClick
  end
  object btnRename: TButton
    Left = 224
    Top = 72
    Width = 81
    Height = 25
    Caption = 'Rename'
    TabOrder = 3
    OnClick = btnRenameClick
  end
  object btnCreate: TButton
    Left = 224
    Top = 104
    Width = 81
    Height = 25
    Caption = 'Create'
    TabOrder = 4
    OnClick = btnCreateClick
  end
  object btnDelete: TButton
    Left = 224
    Top = 136
    Width = 81
    Height = 25
    Caption = 'Delete'
    TabOrder = 5
    OnClick = btnDeleteClick
  end
  object tvFolders: TTreeView
    Left = 8
    Top = 8
    Width = 201
    Height = 321
    HideSelection = False
    Indent = 19
    RightClickSelect = True
    TabOrder = 6
  end
  object OEAPI: TOEAPIObj
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    OnToolbarButtonClicked = OnOEAPIToolbarButtonClicked
    Left = 280
    Top = 248
  end
  object OEFolderManager: TOEFolderManager
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    Left = 280
    Top = 280
  end
  object OEToolbar: TOEToolbar
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    OnButtonClicked = OnToolbarButtonClicked
    Left = 248
    Top = 280
  end
  object OEButton: TOEButton
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    OnClick = OnLastButtonClick
    Left = 216
    Top = 280
  end
  object OEAPIInit: TOEAPIInit
    AutoConnect = True
    ConnectKind = ckRunningOrNew
    OnInitOEAPI = OnInitOEAPI
    OnShutdownOEAPI = OnShutdownOEAPI
    Left = 248
    Top = 216
  end
end
