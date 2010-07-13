VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMessageOps 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Messages"
   ClientHeight    =   7320
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7560
   Icon            =   "frmMessageOps.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7320
   ScaleWidth      =   7560
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnOpenMsg 
      Caption         =   "&Open Msgs"
      Height          =   495
      Left            =   6600
      TabIndex        =   12
      Top             =   6720
      Width           =   855
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   4920
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnSaveToFile 
      Caption         =   "Save to &File"
      Height          =   495
      Left            =   6600
      TabIndex        =   11
      Top             =   3720
      Width           =   855
   End
   Begin VB.CommandButton ForwardMsg 
      Caption         =   "&Forward"
      Height          =   495
      Left            =   6600
      TabIndex        =   10
      Top             =   5520
      Width           =   855
   End
   Begin VB.CommandButton ViewAttachs 
      Caption         =   "View &Attachs"
      Height          =   495
      Left            =   6600
      TabIndex        =   9
      Top             =   4920
      Width           =   855
   End
   Begin VB.CommandButton ViewBodies 
      Caption         =   "View &Bodies"
      Height          =   495
      Left            =   6600
      TabIndex        =   8
      Top             =   4320
      Width           =   855
   End
   Begin VB.CommandButton SelectMessages 
      Caption         =   "&Select Msgs"
      Enabled         =   0   'False
      Height          =   495
      Left            =   6600
      TabIndex        =   7
      Top             =   6120
      Width           =   855
   End
   Begin VB.CommandButton View 
      Caption         =   "&View Source"
      Height          =   495
      Left            =   6600
      TabIndex        =   6
      Top             =   3120
      Width           =   855
   End
   Begin VB.CommandButton MarkAsUnread 
      Caption         =   "Mark &Unread"
      Height          =   495
      Left            =   6600
      TabIndex        =   5
      Top             =   2520
      Width           =   855
   End
   Begin VB.CommandButton MarkAsRead 
      Caption         =   "Mark &Read"
      Height          =   495
      Left            =   6600
      TabIndex        =   4
      Top             =   1920
      Width           =   855
   End
   Begin VB.CommandButton Move 
      Caption         =   "&Move"
      Height          =   495
      Left            =   6600
      TabIndex        =   3
      Top             =   1320
      Width           =   855
   End
   Begin VB.CommandButton Copy 
      Caption         =   "&Copy"
      Height          =   495
      Left            =   6600
      TabIndex        =   2
      Top             =   720
      Width           =   855
   End
   Begin VB.CommandButton Delete 
      Caption         =   "&Delete"
      Height          =   495
      Left            =   6600
      TabIndex        =   1
      Top             =   120
      Width           =   855
   End
   Begin VB.ListBox MessageList 
      Height          =   7080
      Left            =   120
      MultiSelect     =   2  'Extended
      TabIndex        =   0
      Top             =   120
      Width           =   6375
   End
End
Attribute VB_Name = "frmMessageOps"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Dim Folder As OEFolder
Dim FolderSelectionPending As Boolean
Dim MsgSelectionPending As Boolean


Private WithEvents Oeapi As OEAPIObj
Attribute Oeapi.VB_VarHelpID = -1

Option Explicit

'Private Declare Function GetOpenFileName Lib "comdlg32.dll" Alias _
'  "GetOpenFileNameA" (pOpenfilename As OPENFILENAME) As Long
Private Declare Function GetSaveFileName Lib "comdlg32.dll" Alias _
  "GetSaveFileNameA" (pOpenfilename As OPENFILENAME) As Long
  
  
Private Declare Sub Sleep Lib "kernel32" (ByVal secs As Long)

Private Type OPENFILENAME
  lStructSize As Long
  hwndOwner As Long
  hInstance As Long
  lpstrFilter As String
  lpstrCustomFilter As String
  nMaxCustFilter As Long
  nFilterIndex As Long
  lpstrFile As String
  nMaxFile As Long
  lpstrFileTitle As String
  nMaxFileTitle As Long
  lpstrInitialDir As String
  lpstrTitle As String
  flags As Long
  nFileOffset As Integer
  nFileExtension As Integer
  lpstrDefExt As String
  lCustData As Long
  lpfnHook As Long
  lpTemplateName As String
End Type

Private Sub AddMessage(m As OEMessage)
    Dim item As String
    Dim msgUnread As String
    Dim Index As Integer
    
    If (m.GetState And OE_MSG_UNREAD) <> 0 Then
        msgUnread = "Unread "
    Else
        msgUnread = ""
    End If
    
    Index = MessageList.ListCount
    item = m.GetID & " " & msgUnread & m.GetDisplayFrom & " " & m.GetDisplayTo & " " & m.GetNormalSubject
    Me.MessageList.AddItem item, Index
    
    MessageList.ItemData(Index) = m.GetID
    
    If Me.SelectMessages.Enabled Then
        If IsSelected(m.GetID) Then
            MessageList.Selected(Index) = True
        End If
    End If
End Sub

Private Sub btnOpenMsg_Click()
    Dim i As Integer

    If Not Folder Is Nothing Then
        FolderSelectionPending = False
        MsgSelectionPending = False
        
        If Folder.GetID <> Oeapi.GetSelectedFolderID Then
            Oeapi.SetSelectedFolderID Folder.GetID
            FolderSelectionPending = True
        End If
    End If
    
    If Not FolderSelectionPending Then
        OpenSelectedMessages
    End If
End Sub

Private Sub OpenSelectedMessages()
    Dim i As Integer
    
    FolderSelectionPending = False
    
    For i = 0 To MessageList.ListCount - 1
        If MessageList.Selected(i) Then
            Oeapi.SetSelectedMessageID MessageList.ItemData(i)
            Oeapi.OpenCurrentMessage
        End If
    Next
End Sub

Private Sub btnSaveToFile_Click()
    Dim OpenFile As OPENFILENAME
    Dim lReturn As Long
    Dim sFilter As String
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        OpenFile.lStructSize = Len(OpenFile)
        OpenFile.hwndOwner = Me.hwnd
        OpenFile.hInstance = App.hInstance
        sFilter = "All Files (*.*)" & Chr(0) & "*.*" & Chr(0)
        OpenFile.lpstrFilter = sFilter
        OpenFile.nFilterIndex = 1
        OpenFile.nFilterIndex = 0
        OpenFile.lpstrFile = String(257, 0)
        OpenFile.nMaxFile = Len(OpenFile.lpstrFile) - 1
        OpenFile.lpstrFileTitle = OpenFile.lpstrFile
        OpenFile.nMaxFileTitle = OpenFile.nMaxFile
        OpenFile.lpstrInitialDir = "C:\"
        OpenFile.lpstrTitle = "Input file name and extension"
        OpenFile.flags = cdlOFNExplorer
        lReturn = GetSaveFileName(OpenFile)
        If lReturn <> 0 Then
           m.SaveAsFile Trim(OpenFile.lpstrFile)
        End If
    End If
End Sub

Private Function IsSelected(msgId As Long) As Boolean
    Dim selMsgId As Long
    
    IsSelected = False
    
    selMsgId = Oeapi.GetFirstSelectedMessageID()
    While selMsgId <> -1
        If selMsgId = msgId Then
            IsSelected = True
        End If
        selMsgId = Oeapi.GetNextSelectedMessageID()
    Wend
End Function

Private Sub Oeapi_OnFolderSelectionChanged(ByVal folderId As Long)
    If folderId <> -1 And Me.SelectMessages.Enabled Then
        SetFolderId folderId
    End If
End Sub

Private Sub Oeapi_OnMessageSelectionChanged()
    Dim i As Integer
      
    
    If Me.SelectMessages.Enabled Then
        For i = 0 To MessageList.ListCount - 1
           MessageList.Selected(i) = IsSelected(MessageList.ItemData(i))
        Next
    End If

    If FolderSelectionPending Then
        OpenSelectedMessages
    End If
End Sub

Private Sub Copy_Click()
    Dim m As OEMessage
    Dim newFolderParent As String
    Dim newFolder As OEFolder
    Dim fm As OEFolderManager
    Dim fs As frmFolderSelect
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        Set fs = New frmFolderSelect
        fs.Show vbModal
        newFolderParent = fs.GetSelection
        If newFolderParent <> "" Then
            Set newFolder = frmFolders.GetFolder(newFolderParent)
            If Not newFolder Is Nothing Then
                Set fm = New OEFolderManager
                fm.CopyMessage Folder.GetID, newFolder.GetID, m.GetID
            End If
        End If
        
        Unload fs
    End If
End Sub

Private Sub delete_Click()
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        m.delete True
'        folder.DeleteMessage m.GetID True
    End If
End Sub

Private Sub ForwardMsg_Click()
    Dim m As OEMessage
    Dim frwMsg As OEMessage
    Dim auxStr As String
    Dim msgForm As New frmCreateMsg
    Dim frwBodyStr As String
    Dim bodyStr As String
    Dim bodyHandle As Long
    Dim exist As Long
    Dim bodyPos As Long
    Dim rootBody As Long

    Set m = GetSelMessage
    If Not m Is Nothing Then
        msgForm.Caption = "Forward Message"
        msgForm.OKButton.Caption = "Send"
        msgForm.Body.Enabled = False
        msgForm.Subject.text = "Fw: " + m.GetNormalSubject
        msgForm.Show vbModal

        If Not msgForm.GetCancel Then
            ' the folder parameter is not used as the message is not commited
            Set frwMsg = m.Clone(m.GetFolderID)
            
            rootBody = frwMsg.GetBodyHandle(0, OE_IBL_ROOT)
            
            frwMsg.SetBodyProp rootBody, OE_PID_HDR_SUBJECT, msgForm.Subject.text
            frwMsg.SetBodyProp rootBody, OE_PID_HDR_FROM, msgForm.From.text
            frwMsg.SetBodyProp rootBody, OE_PID_HDR_TO, msgForm.To.text
            frwMsg.Send
        End If

        Unload msgForm
    End If
End Sub

Private Sub Form_Load()
    frmFolders.SetTopMostWindow Me.hwnd, True
    FolderSelectionPending = False
End Sub

Public Sub SetHandleSelection()
    Set Oeapi = New OEAPIObj
    Me.SelectMessages.Enabled = True
    
    Oeapi_OnFolderSelectionChanged Oeapi.GetSelectedFolderID
End Sub

Public Sub SetFolderId(folderId As Long)
    Dim f As OEFolder
    Dim fm As New OEFolderManager
    
    If folderId = -1 Then
        MessageList.Clear
    Else
        Set f = fm.GetFolder(folderId)
        SetFolder f, Oeapi
    End If
End Sub

Public Sub SetFolder(f As OEFolder, api As OEAPIObj)
    Dim m As OEMessage

    Set Folder = f
    
    MessageList.Clear
    
    If Not Folder Is Nothing Then
        Set m = f.GetFirstMessage
        While Not m Is Nothing
            AddMessage m
            Set m = f.GetNextMessage
        Wend
    End If
    
    Set Oeapi = api
    Me.btnOpenMsg.Enabled = (Not api Is Nothing)
End Sub

Private Sub MarkAsRead_Click()
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        m.MarkAsRead
'        folder.MarkAsRead m.GetID
    End If
End Sub

Private Sub MarkAsUnread_Click()
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        m.MarkAsUnread
'        folder.MarkAsUnread m.GetID
    End If
End Sub

Private Sub Move_Click()
    Dim m As OEMessage
    Dim newFolderParent As String
    Dim newFolder As OEFolder
    Dim fm As OEFolderManager
    Dim fs As frmFolderSelect

    Set m = GetSelMessage
    If Not m Is Nothing Then
        Set fs = New frmFolderSelect
        fs.Show vbModal
        newFolderParent = fs.GetSelection
        If newFolderParent <> "" Then
            Set newFolder = frmFolders.GetFolder(newFolderParent)
            If Not newFolder Is Nothing Then
                Set fm = New OEFolderManager
                fm.MoveMessage Folder.GetID, newFolder.GetID, m.GetID
            End If
        End If
        Unload fs
    End If
End Sub

Private Sub PlainBody_Click()
    Dim bodyHandle As Long
    Dim m As OEMessage
    Dim msgForm As New frmMessage
    
    Set m = GetSelMessage

    bodyHandle = m.GetPlainBody
    If m.IsBodyContentType(bodyHandle, "text", "plain") = False Then
        MsgBox "Plain body error"
        Exit Sub
    End If

    If Not m Is Nothing Then
        msgForm.bodyStr = m.GetBodyText(bodyHandle)
        msgForm.next.Visible = False
        msgForm.delete.Visible = False
        msgForm.Show vbModal
        m.SetBodyText bodyHandle, msgForm.bodyStr.text, "text", "plain"
        Unload msgForm
        m.Commit
    End If
End Sub

Private Sub ViewAttachs_Click()
    Dim msgAttachs As New frmMessageAttachs
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        msgAttachs.SetMessage m
        msgAttachs.Show vbModal
    End If
End Sub

Private Sub ViewBodies_Click()
    Dim msgProps As New frmMessageProps
    Dim m As OEMessage
    
    Set m = GetSelMessage
    If Not m Is Nothing Then
        msgProps.SetMessage m
        msgProps.Show vbModal
    End If
End Sub

Private Sub SelectMessages_Click()
    Dim i As Integer
    Dim first As Boolean

    first = True

    For i = 0 To MessageList.ListCount - 1
        If MessageList.Selected(i) Then
            If first Then
                Oeapi.SetSelectedMessageID MessageList.ItemData(i)
                first = False
            Else
                Oeapi.AddSelectedMessageID MessageList.ItemData(i)
            End If
        End If
    Next
End Sub

Private Sub View_Click()
    Dim msgForm As New frmMessage
    Dim m As OEMessage

    Set m = GetSelMessage

    If Not m Is Nothing Then
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
End Sub

Private Function GetSelMessage() As OEMessage
    Dim id As String
    Dim sel As String
    Dim i As Integer
    
    sel = Me.MessageList.List(Me.MessageList.ListIndex)
    If sel = "" Then
        Set GetSelMessage = Nothing
    Else
        id = Left(sel, InStr(1, sel, " ", 1))
        i = CInt(id)
        Set GetSelMessage = Folder.GetMessage(i)
    End If
        
End Function

