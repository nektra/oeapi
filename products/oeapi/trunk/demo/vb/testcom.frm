VERSION 5.00
Begin VB.Form frmFolders 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "OEAPI Not Loaded"
   ClientHeight    =   6945
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7680
   Icon            =   "testcom.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6945
   ScaleWidth      =   7680
   Begin VB.CheckBox chkSetTopWindow 
      Caption         =   "Set windows as topmost."
      Enabled         =   0   'False
      Height          =   255
      Left            =   120
      TabIndex        =   48
      Top             =   6600
      Visible         =   0   'False
      Width           =   2895
   End
   Begin VB.CommandButton btnOpenMsg 
      Caption         =   "Open Current Message"
      Enabled         =   0   'False
      Height          =   615
      Left            =   3240
      TabIndex        =   47
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton btnComposeMsg 
      Caption         =   "Compose Message"
      Enabled         =   0   'False
      Height          =   615
      Left            =   4440
      TabIndex        =   46
      Top             =   6120
      Width           =   855
   End
   Begin VB.CommandButton btnMsgWnds 
      Caption         =   "View Msg Wnd"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3360
      TabIndex        =   45
      Top             =   5280
      Width           =   1815
   End
   Begin VB.CommandButton btnHideButton 
      Caption         =   "Hide"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3240
      TabIndex        =   44
      ToolTipText     =   "Hide the button specified by index"
      Top             =   3480
      Width           =   975
   End
   Begin VB.CommandButton btnShowButton 
      Caption         =   "Show"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4320
      TabIndex        =   43
      ToolTipText     =   "Show the button specified by index"
      Top             =   3480
      Width           =   975
   End
   Begin VB.CommandButton btnRemoveButton 
      Caption         =   "Remove"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3360
      TabIndex        =   42
      ToolTipText     =   "Remove the button specified by index"
      Top             =   4440
      Width           =   1815
   End
   Begin VB.CommandButton btnEnableButton 
      Caption         =   "Enable"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3240
      TabIndex        =   41
      ToolTipText     =   "Enable the button specified by index"
      Top             =   3960
      Width           =   975
   End
   Begin VB.CommandButton btnDisableButton 
      Caption         =   "Disable"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4320
      TabIndex        =   40
      ToolTipText     =   "Disable the button specified by index"
      Top             =   3960
      Width           =   975
   End
   Begin VB.CommandButton btnAddBtnSeparator 
      Caption         =   "Add Sep"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4320
      TabIndex        =   39
      Top             =   1680
      Width           =   975
   End
   Begin VB.CommandButton btnAddButton 
      Caption         =   "Add"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3240
      TabIndex        =   38
      ToolTipText     =   "Add a button to the last created toolbar"
      Top             =   1680
      Width           =   975
   End
   Begin VB.CommandButton btnAddSubButton 
      Caption         =   "Add Sub Button"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3360
      TabIndex        =   37
      ToolTipText     =   "Add a sub button to the last created button"
      Top             =   2160
      Width           =   1815
   End
   Begin VB.CommandButton btnRemoveItem 
      Caption         =   "Remove"
      Enabled         =   0   'False
      Height          =   375
      Left            =   6600
      TabIndex        =   36
      ToolTipText     =   "Remove last created menu item"
      Top             =   1200
      Width           =   855
   End
   Begin VB.CommandButton btnAddMenuItem 
      Caption         =   "Add"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5640
      TabIndex        =   35
      ToolTipText     =   "Create a menu item"
      Top             =   1200
      Width           =   855
   End
   Begin VB.CommandButton btnEnableItem 
      Caption         =   "Enable"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5640
      TabIndex        =   34
      ToolTipText     =   "Enable last created menu item"
      Top             =   2160
      Width           =   855
   End
   Begin VB.CommandButton btnDisableItem 
      Caption         =   "Disable"
      Enabled         =   0   'False
      Height          =   375
      Left            =   6600
      TabIndex        =   33
      ToolTipText     =   "Disable last created menu item"
      Top             =   2160
      Width           =   855
   End
   Begin VB.CommandButton btnAddSubItem 
      Caption         =   "Add Sub"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5640
      TabIndex        =   32
      ToolTipText     =   "Create a sub item in the last created menu item"
      Top             =   2640
      Width           =   855
   End
   Begin VB.CommandButton btnRemoveSubItem 
      Caption         =   "Rem Sub"
      Enabled         =   0   'False
      Height          =   375
      Left            =   6600
      TabIndex        =   31
      ToolTipText     =   "Remove last created sub item"
      Top             =   2640
      Width           =   855
   End
   Begin VB.CommandButton btnAddSeparator 
      Caption         =   "Add Sep"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5640
      TabIndex        =   30
      ToolTipText     =   "Create a separator"
      Top             =   1680
      Width           =   855
   End
   Begin VB.CommandButton btnRemoveSeparator 
      Caption         =   "Rem Sep"
      Enabled         =   0   'False
      Height          =   375
      Left            =   6600
      TabIndex        =   29
      ToolTipText     =   "Remove last created separator"
      Top             =   1680
      Width           =   855
   End
   Begin VB.CommandButton SendMessage 
      Caption         =   "Send Message"
      Height          =   375
      Left            =   5760
      TabIndex        =   28
      Top             =   5880
      Width           =   1575
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete"
      Height          =   375
      Left            =   6600
      TabIndex        =   27
      Top             =   3480
      Width           =   855
   End
   Begin VB.CommandButton btnMove 
      Caption         =   "Move"
      Height          =   375
      Left            =   5640
      TabIndex        =   26
      Top             =   3960
      Width           =   855
   End
   Begin VB.CommandButton btnMessages 
      Caption         =   "Print Messages"
      Height          =   375
      Left            =   5760
      TabIndex        =   25
      Top             =   4440
      Width           =   1575
   End
   Begin VB.CommandButton btnCreateMsg 
      Caption         =   "Create Message"
      Height          =   375
      Left            =   5760
      TabIndex        =   24
      Top             =   4920
      Width           =   1575
   End
   Begin VB.CommandButton btnCreateFolder 
      Caption         =   "Create"
      Height          =   375
      Left            =   6600
      TabIndex        =   23
      Top             =   3960
      Width           =   855
   End
   Begin VB.CommandButton btnSelectedMsgs 
      Caption         =   "View msgs Selection"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5760
      TabIndex        =   22
      ToolTipText     =   "View message selection"
      Top             =   5400
      Width           =   1575
   End
   Begin VB.CommandButton btnSelect 
      Caption         =   "Select"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5760
      TabIndex        =   21
      Top             =   6360
      Width           =   1575
   End
   Begin VB.CommandButton btnRename 
      Caption         =   "Rename"
      Height          =   375
      Left            =   5640
      TabIndex        =   20
      Top             =   3480
      Width           =   855
   End
   Begin VB.CommandButton btnShowToolbar 
      Caption         =   "Show"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3240
      TabIndex        =   19
      Top             =   840
      Width           =   975
   End
   Begin VB.CommandButton btnHideToolbar 
      Caption         =   "Hide"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4320
      TabIndex        =   18
      Top             =   840
      Width           =   975
   End
   Begin VB.Frame Message 
      Caption         =   "Other"
      Height          =   975
      Left            =   3120
      TabIndex        =   17
      Top             =   5880
      Width           =   2295
   End
   Begin VB.Frame Frame4 
      Caption         =   "Message Windows"
      Height          =   735
      Left            =   3120
      TabIndex        =   16
      Top             =   5040
      Width           =   2295
   End
   Begin VB.Frame Frame1 
      Caption         =   "Existing Button"
      Height          =   2175
      Left            =   3120
      TabIndex        =   5
      Top             =   2760
      Width           =   2295
      Begin VB.TextBox Index 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   11274
            SubFormatType   =   1
         EndProperty
         Height          =   285
         Left            =   720
         TabIndex        =   6
         ToolTipText     =   "Specify the index of the button (0 based) to modify"
         Top             =   240
         Width           =   975
      End
      Begin VB.Label Label1 
         Caption         =   "Index:"
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.CommandButton btnRemoveToolbar 
      Caption         =   "Remove"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4320
      TabIndex        =   2
      ToolTipText     =   "Remove last created toolbar"
      Top             =   360
      Width           =   975
   End
   Begin VB.ListBox folderList 
      Height          =   6300
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   2895
   End
   Begin VB.CommandButton btnAddToolbar 
      Caption         =   "Add"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3240
      TabIndex        =   0
      ToolTipText     =   "Add a toolbar to the OE"
      Top             =   360
      Width           =   975
   End
   Begin VB.Frame Toolbar 
      Caption         =   "Toolbar"
      Height          =   1215
      Left            =   3120
      TabIndex        =   3
      Top             =   120
      Width           =   2295
   End
   Begin VB.Frame Folder 
      Caption         =   "Folder"
      Height          =   3615
      Left            =   5520
      TabIndex        =   4
      Top             =   3240
      Width           =   2055
   End
   Begin VB.Frame Frame2 
      Caption         =   "Menu Items"
      Height          =   3015
      Left            =   5520
      TabIndex        =   8
      Top             =   120
      Width           =   2055
      Begin VB.TextBox ItemName 
         Height          =   285
         Left            =   1080
         TabIndex        =   13
         Text            =   "testItem"
         ToolTipText     =   "Name of the new item or sub item to create"
         Top             =   720
         Width           =   855
      End
      Begin VB.TextBox ItemText 
         Height          =   285
         Left            =   1080
         TabIndex        =   11
         Text            =   "Block"
         ToolTipText     =   "The new item is created before the item that contains this string"
         Top             =   480
         Width           =   855
      End
      Begin VB.TextBox MenuIndex 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   11274
            SubFormatType   =   1
         EndProperty
         Height          =   285
         Left            =   1080
         TabIndex        =   9
         Text            =   "4"
         ToolTipText     =   "Index of the menu as you see in OE (0 based)"
         Top             =   240
         Width           =   855
      End
      Begin VB.Label Label4 
         Caption         =   "Name"
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   720
         Width           =   735
      End
      Begin VB.Label Label3 
         Caption         =   "Before"
         Height          =   255
         Left            =   120
         TabIndex        =   12
         Top             =   480
         Width           =   855
      End
      Begin VB.Label Label2 
         Caption         =   "Menu Index"
         Height          =   255
         Left            =   120
         TabIndex        =   10
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "Button"
      Height          =   1215
      Left            =   3120
      TabIndex        =   15
      Top             =   1440
      Width           =   2295
   End
End
Attribute VB_Name = "frmFolders"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents myOEAPIInit As OEAPIInit
Attribute myOEAPIInit.VB_VarHelpID = -1

Private WithEvents myButton As OEButton
Attribute myButton.VB_VarHelpID = -1
Private WithEvents mySubButton As OEButton
Attribute mySubButton.VB_VarHelpID = -1
Private WithEvents myToolbar As OEToolbar
Attribute myToolbar.VB_VarHelpID = -1
Private WithEvents myOEAPI As OEAPIObj
Attribute myOEAPI.VB_VarHelpID = -1
Private WithEvents myMenu As OEMenu
Attribute myMenu.VB_VarHelpID = -1
Private WithEvents myMenuItem As OEMenuItem
Attribute myMenuItem.VB_VarHelpID = -1
Private WithEvents mySubItem As OEMenuItem
Attribute mySubItem.VB_VarHelpID = -1
Private WithEvents mySeparator As OEMenuItem
Attribute mySeparator.VB_VarHelpID = -1
Private myFolman As OEFolderManager
Attribute myFolman.VB_VarHelpID = -1

Private PreviousIdentity As String

'Private WithEvents Folman As OEFolderManager
Private myInboxFolder As OEFolder
Attribute myInboxFolder.VB_VarHelpID = -1

Private wndTopMost As Boolean

Dim SelMsgsForm As frmMessageOps
Public Path As String

Private Declare Function GetCurrentDirectory Lib "kernel32" Alias "GetCurrentDirectoryA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
Private Declare Function Sleep Lib "kernel32" (ByVal secs As Long) As Long

Const SWP_NOMOVE = 2
Const SWP_NOSIZE = 1
Const flags = SWP_NOMOVE Or SWP_NOSIZE
Const HWND_TOPMOST = -1
Const HWND_NOTOPMOST = -2


' Virtual Keys
Const VK_TAB = &H9
Const VK_CONTROL = &H11
Const VK_MENU = &H12
Const VK_SHIFT = &H10

Const VK_RETURN = &HD
Const VK_ESCAPE = &H1B
Const VK_DELETE = &H2E
Const VK_F4 = &H73
Const VK_LEFT = &H25

Private Declare Function SetWindowPos Lib "user32" _
      (ByVal hWnd As Long, _
      ByVal hWndInsertAfter As Long, _
      ByVal X As Long, _
      ByVal Y As Long, _
      ByVal cx As Long, _
      ByVal cy As Long, _
      ByVal wFlags As Long) As Long

Public Function UpdateWindowPosition() As Long
    SetTopMostWindow Me.hWnd, wndTopMost
End Function

Public Function SetTopMostWindow(hWnd As Long, topMost As Boolean) As Long
    If topMost = True Then 'Make the window topmost
        SetTopMostWindow = SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, _
           0, flags)
     Else
        SetTopMostWindow = SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, _
           0, 0, flags)
        SetTopMostWindow = False
     End If
End Function

Private Sub btnAddBtnSeparator_Click()
    If myToolbar Is Nothing Then
        MsgBox "You must create a new toolbar first.", vbOKOnly, "Button creation failed"
        Exit Sub
    End If
    
    Set myButton = New OEButton
    myButton.CreateSeparator myToolbar.GetID()
End Sub

Private Sub btnAddButton_Click()
    Dim imgNormal As String
    Dim imgMouseOver As String

    If myToolbar Is Nothing Then
        MsgBox "You must create a new toolbar first.", vbOKOnly, "Button creation failed"
        Exit Sub
    End If
    
    Set myButton = New OEButton

    imgNormal = Path + "\images\img_normal.bmp"
    imgMouseOver = Path + "\images\img_mouseover.bmp"
    myButton.SetTooltip "This is a tooltip"
    myButton.Create myToolbar.GetID(), "Nektra", imgNormal, imgMouseOver
End Sub

Function GetButtonIndex() As Integer
    Dim ret As Integer
    ret = -1
    If Me.Index.text <> "" Then
        ret = CInt(Me.Index.text)
    End If
    GetButtonIndex = ret
End Function

Private Sub btnMsgWnds_Click()
    Dim msgWndForm As New frmMsgWnds

    msgWndForm.Show vbModal
End Sub

Private Sub btnOpenMsg_Click()
    myOEAPI.OpenCurrentMessage
End Sub

Private Sub btnComposeMsg_Click()
    myOEAPI.ComposeMessage
End Sub

Private Sub btnSelect_Click()
    Dim folName As String
    Dim Folder As OEFolder
    
    If folderList.ListIndex < 0 Or folderList.ListIndex >= folderList.ListCount Then
        MsgBox "You must select a folder first", vbOKOnly, "Select Folder"
        Exit Sub
    End If
    
    folName = Me.folderList.List(Me.folderList.ListIndex)
    Set Folder = GetFolder(folName)
    
    If Not myOEAPI Is Nothing And Not Folder Is Nothing Then
        myOEAPI.SetSelectedFolderID Folder.GetID
    End If
End Sub

Private Sub btnEnableButton_Click()
    Dim Index As Integer
    Dim butId As Long
    Dim i As Integer
    Dim btn As OEButton

    Index = GetButtonIndex()
    If Index = -1 Then
        MsgBox "Invalid index."
        Exit Sub
    End If
    
    If myToolbar Is Nothing Then
        MsgBox "There is no Toolbar."
        Exit Sub
    End If
    i = 0

    Set btn = myToolbar.GetButtonByIndex(Index)
    If Not btn Is Nothing Then
        btn.SetEnabled True
    End If

'    butId = myToolbar.GetFirstButtonID
'    While i <> Index And butId <> -1
'        butId = myToolbar.GetNextButtonID
'        i = i + 1
'    Wend
'    If butId <> -1 Then
'        Dim btn As OEButton
'        Set btn = myToolbar.GetButton(butId)
'        btn.SetEnabled True
'    End If
End Sub


Private Sub btnRemoveButton_Click()
    Dim Index As Integer
    Dim butId As Long
    Dim i As Integer
    Dim btn As OEButton
    
    Index = GetButtonIndex()
    If Index = -1 Then
        MsgBox "Invalid index."
        Exit Sub
    End If
    
    If myToolbar Is Nothing Then
        MsgBox "There is no Toolbar."
        Exit Sub
    End If
    i = 0
    
    Set btn = myToolbar.GetButtonByIndex(Index)
    If Not btn Is Nothing Then
        btn.Destroy
    End If
End Sub

Private Sub chkSetTopWindow_Click()
    wndTopMost = chkSetTopWindow.Value
    UpdateWindowPosition
End Sub

Private Sub SendMessage_Click()
    Dim msgForm As New frmCreateMsg
    Dim msgSource As String
    Dim m As OEMessage
    Dim bodyHandle As Long
    Dim propId As Long
    Dim Folder As OEFolder
    
    msgForm.Caption = "Send Message"
    msgForm.OKButton.Caption = "Send"
    msgForm.Show vbModal
    If Not msgForm.GetCancel Then
        Set m = New OEMessage
        
        bodyHandle = m.InsertBody(0, OE_IBL_ROOT)
        m.SetBodyText bodyHandle, msgForm.Body.text, "text", "plain"
        m.SetBodyProp bodyHandle, OE_PID_HDR_SUBJECT, msgForm.Subject.text
        m.SetBodyProp bodyHandle, OE_PID_HDR_FROM, msgForm.From.text
        m.SetBodyProp bodyHandle, OE_PID_HDR_TO, msgForm.To.text
        
        ' Use IOEAPIObj.SendMessage if OE is running
        If myOEAPI Is Nothing Then
            m.Send
        Else
            Set Folder = myFolman.GetDraftFolder
            
            m.SetFolderId Folder.GetID
            m.Commit

            myOEAPI.SendMessage Folder.GetID, m.GetID
            Set Folder = Nothing
            m.delete True
            
'            myOEAPI.SendAndReceiveMessages
        End If
        Set m = Nothing
    End If
    Set msgForm = Nothing
End Sub

Private Sub btnShowButton_Click()
    Dim Index As Integer
    Dim butId As Long
    Dim i As Integer
    Dim btn As OEButton
    
    Index = GetButtonIndex()
    If Index = -1 Then
        MsgBox "Invalid index."
        Exit Sub
    End If
    
    If myToolbar Is Nothing Then
        MsgBox "There is no Toolbar."
        Exit Sub
    End If
    i = 0
    
    Set btn = myToolbar.GetButtonByIndex(Index)
    If Not btn Is Nothing Then
        btn.SetShown True
    End If
End Sub


Private Sub btnHideButton_Click()
    Dim Index As Integer
    Dim butId As Long
    Dim i As Integer
    Dim btn As OEButton
    
    Index = GetButtonIndex()
    If Index = -1 Then
        MsgBox "Invalid index."
        Exit Sub
    End If
    
    If myToolbar Is Nothing Then
        MsgBox "There is no Toolbar."
        Exit Sub
    End If
    i = 0
    
    Set btn = myToolbar.GetButtonByIndex(Index)
    If Not btn Is Nothing Then
        btn.SetShown False

        ' test of dinamic change of button properties
'        btn.SetName "Nektra"
        
'        Dim img As String
        
'        img = path + "\images\img_mouseover.bmp"
'        btn.SetImageNormal img
'        img = path + "\images\img_normal.bmp"
'        btn.SetImageMouseOver img

    End If
End Sub


Private Sub btnDisableButton_Click()
    Dim Index As Integer
    Dim butId As Long
    Dim i As Integer
    Dim btn As OEButton
    
    Index = GetButtonIndex()
    If Index = -1 Then
        MsgBox "Invalid index."
        Exit Sub
    End If
    
    If myToolbar Is Nothing Then
        MsgBox "There is no Toolbar."
        Exit Sub
    End If
    i = 0
    
    Set btn = myToolbar.GetButtonByIndex(Index)
    If Not btn Is Nothing Then
        btn.SetEnabled False
    End If
End Sub


Private Sub btnAddSubButton_Click()
    Dim imgNormal As String
    Dim imgMouseOver As String
    
    If myButton Is Nothing Then
        MsgBox "You must have created a button first.", vbOKOnly, "Sub button creation failed"
        Exit Sub
    End If
    imgNormal = Path + "\images\img_normal.bmp"
    imgMouseOver = Path + "\images\img_mouseover.bmp"
    Set mySubButton = myButton.CreateSubButton("Nektra", imgNormal, imgMouseOver)
End Sub

Private Sub btnAddMenuItem_Click()
    Dim Index As Integer
    Dim subStr As String
    Dim name As String
    
    If Me.MenuIndex.text <> "" Then
        Index = CInt(Me.MenuIndex.text)
    Else
        MsgBox "Invalid Index"
        Exit Sub
    End If
    
    name = Me.ItemName.text

    If myOEAPI Is Nothing Then
        Set myOEAPI = New OEAPIObj
    End If
    
    Set myMenu = myOEAPI.GetMenu(Index)
    If myMenu Is Nothing Then
        MsgBox "Invalid Index"
        Exit Sub
    End If
    
    subStr = Me.ItemText.text
    
    If subStr = "" Then
        Set myMenuItem = myMenu.CreateItem(name)
    Else
        Set myMenuItem = myMenu.CreateItemBefore(name, subStr, True)
    End If
End Sub

Private Sub btnRemoveItem_Click()
    If Not myMenuItem Is Nothing Then
        myMenuItem.Destroy
    End If
End Sub

Private Sub btnAddSeparator_Click()
    Dim Index As Integer
    Dim subStr As String
    
    If Me.MenuIndex.text <> "" Then
        Index = CInt(Me.MenuIndex.text)
    Else
        MsgBox "Invalid Index"
        Exit Sub
    End If
    
    If myOEAPI Is Nothing Then
        Set myOEAPI = New OEAPIObj
    End If
    
    Set myMenu = myOEAPI.GetMenu(Index)
    If myMenu Is Nothing Then
        MsgBox "Invalid Index"
        Exit Sub
    End If
    
    subStr = Me.ItemText.text
    
    If subStr = "" Then
        Set mySeparator = myMenu.CreateSeparator()
    Else
        Set mySeparator = myMenu.CreateSeparatorBefore(subStr, True)
    End If
End Sub
Private Sub btnRemoveSeparator_Click()
    If Not mySeparator Is Nothing Then
        mySeparator.Destroy
    End If
End Sub

Private Sub btnEnableItem_Click()
    If Not myMenuItem Is Nothing Then
        myMenuItem.SetEnabled True
    End If
End Sub
Private Sub btnDisableItem_Click()
    If Not myMenuItem Is Nothing Then
        myMenuItem.SetEnabled False
    End If
End Sub
Private Sub btnAddSubItem_Click()
    Dim name As String
    
    If myMenuItem Is Nothing Then
        MsgBox "You have to create a menu item first."
        Exit Sub
    End If
    
    name = Me.ItemName.text

    Set mySubItem = myMenuItem.CreateSubItem(name)
End Sub

Private Sub btnRemoveSubItem_Click()
    If myMenuItem Is Nothing Then
        MsgBox "You have to create a sub menu item first."
        Exit Sub
    End If
    mySubItem.Destroy
End Sub


Private Sub btnCreateFolder_Click()
    Dim Folder As String
    Dim folderObj As OEFolder
    
    Folder = Me.folderList.List(Me.folderList.ListIndex)
    If Folder = "" Then
        MsgBox "You must select a folder first.", vbOKOnly, "Create Folder"
        Exit Sub
    End If
    
    Set folderObj = GetFolder(Folder)
    If Not folderObj Is Nothing Then
        Folder = InputBox("Enter new folder name", "Create Folder")
        folderObj.CreateFolder Folder
    End If
    RefreshFolders
End Sub

Private Sub btnCreateMsg_Click()
    Dim msgForm As New frmCreateMsg
    Dim msgSource As String
    Dim Folder As String
    Dim folderObj As OEFolder
    
    Folder = Me.folderList.List(Me.folderList.ListIndex)
    If Folder = "" Then
        MsgBox "You must select a folder first.", vbOKOnly, "Create message"
        Exit Sub
    End If
    
    Set folderObj = GetFolder(Folder)
    
    If Not folderObj Is Nothing Then
        msgForm.Caption = "New Message in " + Folder
        msgForm.Show vbModal
        If Not msgForm.GetCancel Then
            Dim m As OEMessage
            
            msgSource = msgSource + "From: " + msgForm.From.text + Chr(13) + Chr(10)
            msgSource = msgSource + "To: " + msgForm.To.text + Chr(13) + Chr(10)
            msgSource = msgSource + "Subject: " + msgForm.Subject.text + Chr(13) + Chr(10) + Chr(13) + Chr(10)
            msgSource = msgSource + msgForm.Body.text

            Set m = folderObj.CreateMessage(msgSource)
            If Not m Is Nothing Then
                MsgBox "Message succesfuly created.", vbOKOnly, "Create Message"
            End If
        End If
    End If
End Sub

Private Sub myInboxFolder_OnNewMessage(ByVal msgId As Long)
    If MsgBox("Do you want to display it?", vbYesNo, "New message in folder inbox") = vbYes Then
        Dim m As OEMessage
        
        Set m = myInboxFolder.GetMessage(msgId)
        If Not m Is Nothing Then
            Dim msgForm As frmMessage
            Set msgForm = New frmMessage
            
            msgForm.fromStr.Caption = m.GetDisplayFrom
            msgForm.toStr = m.GetDisplayTo
            msgForm.subjectStr = m.GetSubject
            msgForm.norSubjectStr = m.GetNormalSubject
            msgForm.headerStr = m.GetHeader
            msgForm.bodyStr = m.GetAllBody
            msgForm.next.Visible = False
            msgForm.delete.Visible = False
            msgForm.Show vbModal
            Unload msgForm
            Set msgForm = Nothing
        End If
    End If
End Sub

Private Sub myOEAPI_OnNewMessageInOutbox(ByVal msgId As Long)
    If MsgBox("Do you want to display it?", vbYesNo, "New message in Outbox") = vbYes Then
        Dim msgProps As New frmMessageProps
        Dim m As OEMessage
        Dim outbox As OEFolder
        
        Set outbox = myFolman.GetOutboxFolder
        Set m = outbox.GetMessage(msgId)

        If Not m Is Nothing Then
            msgProps.SetMessage m
            msgProps.Show vbModal
            Set m = Nothing
        End If
        Set outbox = Nothing
        Set msgProps = Nothing
    End If
End Sub

Private Sub myInboxFolder_OnMessageDeleted(ByVal msgId As Long)
    MsgBox "Message deleted in folder inbox"
End Sub

Private Sub btnRename_Click()
    Dim fm As OEFolderManager
    Dim f As OEFolder
    Dim inbox As OEFolder
    Dim n As String
    Dim renFolder As String
    Dim newName As String
    
    If folderList.ListIndex < 0 Or folderList.ListIndex >= folderList.ListCount Then
        MsgBox "You must select a folder first.", vbOKOnly, "Rename Folder"
        Exit Sub
    End If
    
    renFolder = Me.folderList.List(Me.folderList.ListIndex)
    
    Set f = GetFolder(renFolder)

    If Not f.IsNull Then
        newName = InputBox("Enter new folder name", "Rename Folder")
        If newName <> "" Then
            f.Rename (newName)
        End If
    End If
    
    RefreshFolders
End Sub

Private Sub btnDelete_Click()
    Dim fm As OEFolderManager
    Dim f As OEFolder
    Dim a As OEFolder
    Dim inbox As OEFolder
    Dim n As String
    Dim delFolder As String
    Dim id As Long
    
    If folderList.ListIndex < 0 Or folderList.ListIndex >= folderList.ListCount Then
        MsgBox "You must select a folder first.", vbOKOnly, "Delete Folder"
        Exit Sub
    End If
    
    delFolder = Me.folderList.List(Me.folderList.ListIndex)
    
    Set f = GetFolder(delFolder)

    If Not f.IsNull Then
        f.delete
        Set f = Nothing
    End If

    RefreshFolders
End Sub

Private Sub btnMove_Click()
    Dim f As OEFolder
    Dim mvFolder As String
    Dim newFolderParentId As Long
    Dim fs As frmFolderSelect
    
    If folderList.ListIndex < 0 Or folderList.ListIndex >= folderList.ListCount Then
        MsgBox "You must select a folder first.", vbOKOnly, "Move Folder"
        Exit Sub
    End If
    
    mvFolder = Me.folderList.List(Me.folderList.ListIndex)
    
    Set f = GetFolder(mvFolder)
    
    If Not f.IsNull Then
        Set fs = New frmFolderSelect
        fs.Show vbModal
        newFolderParentId = fs.GetSelectionID
        If newFolderParentId = -1 Then
            Exit Sub
        End If
        
        f.Move newFolderParentId
        
        RefreshFolders
    End If
End Sub

Private Sub RefreshFolders()
    Dim fm As OEFolderManager
    Dim f As OEFolder
    Dim n As String
    
    Me.folderList.Clear
    Set fm = New OEFolderManager
    Set f = fm.GetFirstFolder
    
    While Not f Is Nothing
        n = f.name() + " (" + Str(f.GetID) + " )"
        Me.folderList.AddItem n
        
        Set f = fm.GetNextFolder
    Wend
End Sub
Public Function GetFolder(name As String) As OEFolder
    Dim fm As OEFolderManager
    Dim f As OEFolder
    Dim n As String
    
    Set fm = New OEFolderManager
    Set f = fm.GetFirstFolder
    While Not f Is Nothing
        n = f.name() + " (" + Str(f.GetID) + " )"
        If n = name Then
            Set GetFolder = f
            Exit Function
        End If
        Set f = fm.GetNextFolder
    Wend
End Function


Private Sub BrowseInbox()
    ' browse inbox folder
    Dim fm As OEFolderManager
    Dim f As OEFolder
    Dim inbox As OEFolder
    Dim n As String
    
    Set fm = New OEFolderManager
    Set inbox = fm.GetFirstFolder
    Set f = inbox.GetFirstChild
    While Not f Is Nothing
        n = f.name()
        Print n
        Set f = inbox.GetNextChild
    Wend
End Sub

Private Sub Form_Load()
    Dim identity As New OEIdentity
    
    wndTopMost = True
    chkSetTopWindow.Value = 1
    UpdateWindowPosition
    'SetTopMostWindow Me.hwnd, True
    'Path = Space(1024)
    'Size = GetCurrentDirectory(1024, Path)
    Path = App.Path 'Left(Path, Size)
    
    PreviousIdentity = identity.GetCurrentIdentity

'    path = "C:\home\pablo\products\oeapi\demo\vb"

    Set myOEAPI = Nothing
    Set myOEAPIInit = New OEAPIInit
    Set myFolman = New OEFolderManager
    
'    Set InboxFolder = myFolman.GetInboxFolder
    
    Set SelMsgsForm = Nothing
    
    
    RefreshFolders
    
'    set myfolman = Nothing
'    Set InboxFolder = Nothing
End Sub
Private Sub Form_Unload(cancel As Integer)
    Dim auxItem As OEMenuItem
    Dim t As OEToolbar
    Dim toolbarId As Long
    
    Set myFolman = Nothing
'    Set InboxFolder = Nothing
    
    If Not SelMsgsForm Is Nothing Then
        Unload SelMsgsForm
    End If
    
    If Not myOEAPI Is Nothing Then
'       Remove menu items

'        itemId = myOEAPI.GetFirstMenuItemID
'        While itemId <> -1
'            Set auxItem = myOEAPI.GetMenuItem(itemId)
'            If Not auxItem Is Nothing Then
'                auxItem.Destroy
'            End If
'            itemId = myOEAPI.GetFirstMenuItemID
'        Wend
        myOEAPI.RemoveAllMenuItems
        
'       Remove toolbars

'        toolbarId = myOEAPI.GetFirstToolbarID
'        While toolbarId <> -1
'            Set t = myOEAPI.GetToolbar(toolbarId)
'            If Not t Is Nothing Then
'                t.Destroy
'            End If
'            toolbarId = myOEAPI.GetFirstToolbarID
'        Wend
        myOEAPI.RemoveAllToolbars
    End If
End Sub

Private Sub btnMessages_Click()
    Dim f As OEFolder
    Dim n As String
    Dim m As OEMessage
    Dim Folder As String
    Dim msgsForm As New frmMessageOps
    
    Folder = Me.folderList.List(Me.folderList.ListIndex)
    Set f = GetFolder(Folder)
    If Not f Is Nothing Then
        msgsForm.Caption = f.name
        msgsForm.SetFolder f, myOEAPI
        msgsForm.Show vbModal
    End If
End Sub

Private Sub myButton_OnClick()
    MsgBox "Nektra clicked button event"
End Sub

Private Sub btnAddToolbar_Click()
    Dim toolProps As New frmToolbarProps
    
    If myOEAPI Is Nothing Then
        Set myOEAPI = New OEAPIObj
    End If
    
    ' You can create buttons in the OE Toolbar
'    set mytoolbar =myOEAPI.GetOEToolbar
    
    ' or you can create your own toolbar
    toolProps.Show vbModal
    If toolProps.GetOKValue Then
        If toolProps.chkMsgWnd.Value Then
            Dim wndStyle As Oeapi.tagWNDSTYLE
            wndStyle = toolProps.cboToolbarMsgWnd.ItemData(toolProps.cboToolbarMsgWnd.ListIndex)
            If toolProps.OEToolbar.Value Then
                Set myToolbar = myOEAPI.GetOEToolbarInMsgWnd(wndStyle)
            Else
                Set myToolbar = New OEToolbar
                myToolbar.CreateInMsgWnd wndStyle, myOEAPI.GetID
                myToolbar.SetLargeButtons toolProps.LargeBtns.Value
                myToolbar.EnableTooltips toolProps.chkEnableTooltips.Value
            End If
        Else
            If toolProps.OEToolbar.Value Then
                Set myToolbar = myOEAPI.GetOEToolbar
            Else
                ' First set the properties of the toolbar and then create it
                Set myToolbar = New OEToolbar
                myToolbar.SetLargeButtons toolProps.LargeBtns.Value
                myToolbar.EnableTooltips toolProps.chkEnableTooltips.Value
                myToolbar.Create myOEAPI.GetID
            End If
        End If
    End If
End Sub

Private Sub btnRemoveToolbar_Click()
    If myToolbar Is Nothing Then
        MsgBox "No toolbar was created"
        Exit Sub
    End If
    myToolbar.Destroy
End Sub

Private Sub btnShowToolbar_Click()
    If myToolbar Is Nothing Then
        MsgBox "No toolbar was created"
        Exit Sub
    End If
    myToolbar.SetShown True
End Sub
Private Sub btnHideToolbar_Click()
    If myToolbar Is Nothing Then
        MsgBox "No toolbar was created"
        Exit Sub
    End If
    myToolbar.SetShown False
End Sub

Private Sub SubButton_OnClick()
    MsgBox "Nektra clicked sub button event"
End Sub

Private Sub btnSelectedMsgs_Click()
    If myOEAPI Is Nothing Then
        Set myOEAPI = New OEAPIObj
    End If
    
    If SelMsgsForm Is Nothing Then
        Set SelMsgsForm = New frmMessageOps
        SelMsgsForm.SetHandleSelection
        SelMsgsForm.Show vbModal
        Set SelMsgsForm = Nothing
    End If
End Sub
Private Sub myOEAPI_OnMessageDownloadStarted()
'    MsgBox "Download Started"
End Sub
Private Sub myOEAPI_OnMessageDownloadFinished()
'    MsgBox "Download Finished"
End Sub

Private Sub myOEAPI_OnToolbarButtonClicked(ByVal toolbarId As Long, ByVal buttonId As Long)
    MsgBox "Nektra clicked oeapi event"
End Sub
Private Sub myOEAPI_OnMenuItemClicked(ByVal menuId As Long, ByVal itemId As Long)
    MsgBox "testItem clicked oeapi event"
End Sub
Private Sub myOEAPI_OnFolderSelectionChanged(ByVal folderId As Long)
    Dim Folder As OEFolder
    Dim msg As String
    
    msg = "A new folder is selected "
    
    If Not myFolman Is Nothing Then
        Set Folder = myFolman.GetFolder(folderId)

        If Not Folder Is Nothing Then
            msg = msg + Folder.name()
        End If
    End If

    MsgBox msg, vbOKOnly, "Folder Selection changed"
End Sub
Private Sub myToolbar_OnButtonClicked(ByVal buttonId As Long)
    MsgBox "Nektra clicked toolbar event"
End Sub
Private Sub myMenuItem_OnClick()
    MsgBox "testItem clicked item event"
End Sub
Private Sub myMenu_OnItemClicked(ByVal itemId As Long)
    MsgBox "testItem clicked menu event"
End Sub

Private Sub myFolman_OnFolderCreated(ByVal folderId As Long)
    Dim Folder As OEFolder
    Dim msg As String
    
    msg = "A folder was created "
    Set Folder = myFolman.GetFolder(folderId)
    
    If Not Folder.IsNull Then
        msg = msg + Folder.name()
    End If
    MsgBox msg, vbOKOnly, "Folder Creation"
End Sub

Private Sub myFolman_OnFolderDeleted(ByVal folderId As Long)
    Dim Folder As OEFolder
    Dim msg As String
    
    msg = "A folder was deleted "
    Set Folder = myFolman.GetFolder(folderId)
    
    If Not Folder.IsNull Then
        msg = msg + Folder.name()
    End If
    MsgBox msg, vbOKOnly, "Folder Deletion"
End Sub

Private Sub myFolman_OnFolderRenamed(ByVal folderId As Long)
    Dim Folder As OEFolder
    Dim msg As String
    
    msg = "A folder was renamed "
    Set Folder = myFolman.GetFolder(folderId)
    
    If Not Folder.IsNull Then
        msg = msg + Folder.name()
    End If
    MsgBox msg, vbOKOnly, "Folder renamed"
End Sub

Private Sub myFolman_OnFolderMoved(ByVal folderId As Long)
    Dim Folder As OEFolder
    Dim msg As String
    
    msg = "A folder was moved "
    Set Folder = myFolman.GetFolder(folderId)
    
    If Not Folder.IsNull Then
        msg = msg + Folder.name()
    End If
    MsgBox msg, vbOKOnly, "Folder moved"
End Sub

'Private Sub myFolman_OnFolderUnreadChanged(ByVal folderId As Long)
'    Dim Folder As OEFolder
'    Dim msg As String
'
'    msg = "Unread changed "
'    set myfolder =myFolman.GetFolder(folderId)
'
'    If Not Folder.IsNull Then
'        msg = msg + Folder.name()
'    End If
'    MsgBox msg, vbOKOnly, "Folder Unread changed"
'End Sub

Private Sub myOEAPI_OnDatabaseChange(ByVal dt As tagOE_DATABASE_TRANSACTION, ByVal folderId As Long, ByVal objId As Long, ByVal parId As Long)
    Dim Folder As OEFolder
    Dim folName As String
    Dim title As String
    
    If dt <> OE_TR_DELETE_FOLDER Then
        Set Folder = myFolman.GetFolder(folderId)
        folName = Folder.name
    End If

    Select Case dt
    Case OE_TR_DELETE_FOLDER
        title = "Folder deleted " + Str(folderId)
        
        Set Folder = myFolman.GetFolder(objId)
        If Not Folder Is Nothing Then
            title = title + " Parent " + Folder.name
        End If
        
        MsgBox title, vbOKOnly, "Folder Deleted"

    Case OE_TR_INSERT_FOLDER
        title = "New folder " + folName
        
        Set Folder = myFolman.GetFolder(objId)
        If Not Folder Is Nothing Then
            title = title + " Parent " + Folder.name
        End If
        
        MsgBox title, vbOKOnly, "New Folder"
    Case OE_TR_INSERT_MESSAGE
        If Not Folder Is Nothing Then
            If Folder.IsDraft = 0 And Folder.IsOutbox = 0 Then
                title = "New message in folder "
                If MsgBox("Do you want to display it?", vbYesNo, title + folName) = vbYes Then
                     Dim msgProps As New frmMessageProps
                     Dim m As OEMessage
                     Dim outbox As OEFolder
                     
                     Set m = Folder.GetMessage(objId)
                    
                     If Not m Is Nothing Then
                         msgProps.SetMessage m
                         msgProps.Show vbModal
                         Set m = Nothing
                     End If
                     Set msgProps = Nothing
                End If
            End If
        End If
    Case OE_TR_DELETE_MESSAGE
        title = "Message deleted in folder " + folName
        MsgBox title, vbOKOnly, "Message Deleted"
    End Select
End Sub

Private Sub myFolman_OnNewMessage(ByVal folderId As Long, ByVal msgId As Long)
    Dim Folder As OEFolder
    Dim folName As String
    
    Set Folder = myFolman.GetFolder(folderId)
    
    If Not Folder Is Nothing Then
        ' inbox folder is handled separatly
        
        If Folder.IsInbox = 0 Then
            folName = Folder.name

            If MsgBox("Do you want to display it?", vbYesNo, "New message in folder " + folName) = vbYes Then
                Dim m As OEMessage

                Set m = Folder.GetMessage(msgId)
                If Not m Is Nothing Then
                    Dim msgForm As frmMessage
                    Set msgForm = New frmMessage
                    
                    msgForm.fromStr.Caption = m.GetDisplayFrom
                    msgForm.toStr = m.GetDisplayTo
                    msgForm.subjectStr = m.GetSubject
                    msgForm.norSubjectStr = m.GetNormalSubject
                    msgForm.headerStr = m.GetHeader
                    msgForm.bodyStr = m.GetAllBody
                    msgForm.next.Visible = False
                    msgForm.delete.Visible = False
                    msgForm.Show vbModal
                    Unload msgForm
                End If
            End If
        End If
    End If
End Sub

Private Sub myOEAPIInit_OnInitOEAPI()
    Dim identity As New OEIdentity
    Dim newIdentity As String

    Me.Caption = "OEAPI Loaded"
    If myOEAPI Is Nothing Then
        Set myOEAPI = New OEAPIObj

        newIdentity = identity.GetCurrentIdentity

        If newIdentity <> PreviousIdentity Then
            PreviousIdentity = newIdentity

            ' create all the objects again to force reload of the
            ' oecom library.
            ' otherwise, a identity change will not be handled
            Set myFolman = Nothing
    '        Set InboxFolder = Nothing
    
            Set myFolman = New OEFolderManager
    '        myFolman.ActivateGlobalNotification
    
            ' get the new inbox folder if the identity changed
    '        Set InboxFolder = myFolman.GetInboxFolder
    
            RefreshFolders
        End If
    End If

    btnAddToolbar.Enabled = True
    btnRemoveToolbar.Enabled = True
    btnShowToolbar.Enabled = True
    btnHideToolbar.Enabled = True
    btnAddButton.Enabled = True
    btnAddSubButton.Enabled = True
    btnAddBtnSeparator.Enabled = True
    btnHideButton.Enabled = True
    btnShowButton.Enabled = True
    btnEnableButton.Enabled = True
    btnDisableButton.Enabled = True
    btnRemoveButton.Enabled = True
    btnAddMenuItem.Enabled = True
    btnRemoveItem.Enabled = True
    btnAddSeparator.Enabled = True
    btnRemoveSeparator.Enabled = True
    btnEnableItem.Enabled = True
    btnDisableItem.Enabled = True
    btnAddSubItem.Enabled = True
    btnRemoveSubItem.Enabled = True
    
    ' Begin Professional Version Or Enterprise Version
    btnSelectedMsgs.Enabled = True
    btnSelect.Enabled = True
    btnOpenMsg.Enabled = True
    btnComposeMsg.Enabled = True
    ' End Professional Version Or Enterprise Version
    
    ' Begin Enterprise Version
    btnMsgWnds.Enabled = True
    ' End Enterprise Version
End Sub

Private Sub myOEAPIInit_OnShutdownOEAPI()
    Dim itemId As Long
            
    btnAddToolbar.Enabled = False
    btnRemoveToolbar.Enabled = False
    btnShowToolbar.Enabled = False
    btnHideToolbar.Enabled = False
    btnAddButton.Enabled = False
    btnAddSubButton.Enabled = False
    btnAddBtnSeparator.Enabled = False
    btnHideButton.Enabled = False
    btnShowButton.Enabled = False
    btnEnableButton.Enabled = False
    btnDisableButton.Enabled = False
    btnRemoveButton.Enabled = False
    btnAddMenuItem.Enabled = False
    btnRemoveItem.Enabled = False
    btnAddSeparator.Enabled = False
    btnRemoveSeparator.Enabled = False
    btnEnableItem.Enabled = False
    btnDisableItem.Enabled = False
    btnAddSubItem.Enabled = False
    btnRemoveSubItem.Enabled = False

    ' Begin Professional Version Or Enterprise Version
    btnSelectedMsgs.Enabled = False
    btnSelect.Enabled = False
    btnOpenMsg.Enabled = False
    btnComposeMsg.Enabled = False
    ' End Professional Version Or Enterprise Version

    ' Begin Enterprise Version
    btnMsgWnds.Enabled = False
    ' End Enterprise Version

    Me.Caption = "OEAPI Not Loaded"
    myOEAPI.RemoveAllToolbars
    myOEAPI.RemoveAllMenuItems
    Set myOEAPI = Nothing
    Set myToolbar = Nothing
    Set myButton = Nothing
    Set mySubButton = Nothing
    Set myMenu = Nothing
    Set myMenuItem = Nothing
    Set mySubItem = Nothing
    Set mySeparator = Nothing
End Sub

