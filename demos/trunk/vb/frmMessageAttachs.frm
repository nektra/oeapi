VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMessageAttachs 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message Attachments"
   ClientHeight    =   2640
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3900
   Icon            =   "frmMessageAttachs.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2640
   ScaleWidth      =   3900
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog fileDlg 
      Left            =   3360
      Top             =   2160
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton SaveChanges 
      Caption         =   "Save Changes"
      Height          =   375
      Left            =   2520
      TabIndex        =   3
      ToolTipText     =   "Commit the addition of attachments"
      Top             =   1080
      Width           =   1335
   End
   Begin VB.CommandButton AddAttach 
      Caption         =   "Add Attach"
      Height          =   375
      Left            =   2520
      TabIndex        =   2
      ToolTipText     =   "Add a attachment to the message. Press 'Save Change' to confirm the changes"
      Top             =   600
      Width           =   1335
   End
   Begin VB.CommandButton SaveDisk 
      Caption         =   "Save to disk"
      Height          =   375
      Left            =   2520
      TabIndex        =   1
      ToolTipText     =   "Save the selected attachment to the disk"
      Top             =   120
      Width           =   1335
   End
   Begin VB.ListBox lbAttach 
      Height          =   2400
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2295
   End
End
Attribute VB_Name = "frmMessageAttachs"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim curMsg As OEMessage

Private Function GetSelBody() As Long
    Dim id As String
    Dim sel As String
    Dim Index As Integer
    
    Index = Me.lbAttach.ListIndex
    If Index = -1 Then
        GetSelBody = 0
    Else
        GetSelBody = Me.lbAttach.ItemData(Index)
    End If
End Function


Public Sub SetMessage(msg As OEMessage)
    Set curMsg = msg
    
    RefreshAttachs
End Sub

Private Sub RefreshAttachs()
    Dim bodyHandle As Long
    Dim Index As Integer
    Dim attachName As String
    
    Me.lbAttach.Clear
    
    Index = 0
    bodyHandle = curMsg.GetFirstAttachment
    While bodyHandle <> 0
        attachName = curMsg.GetFilename(bodyHandle)
        If attachName = "" Then
            attachName = curMsg.GetBodyDisplayName(bodyHandle)
        End If

        Me.lbAttach.AddItem attachName, Index
        Me.lbAttach.ItemData(Index) = bodyHandle

        Index = Index + 1
        bodyHandle = curMsg.GetNextAttachment
    Wend
End Sub

Private Sub AddAttach_Click()
    fileDlg.DialogTitle = "Attach File"
    fileDlg.ShowOpen
    If Not fileDlg.CancelError Then
        If curMsg.AttachFile(fileDlg.FileName) = 0 Then
            MsgBox "Cannot open file"
        Else
            RefreshAttachs
        End If
    End If
End Sub

Private Sub Form_Load()
    frmFolders.SetTopMostWindow Me.hwnd, True
End Sub

Private Sub SaveChanges_Click()
    curMsg.Commit
End Sub

Private Sub SaveDisk_Click()
    Dim bodyHandle As Long
    
    bodyHandle = GetSelBody
    If bodyHandle = 0 Then
        MsgBox "Select a attachment."
        Exit Sub
    End If

    fileDlg.DialogTitle = "Save to Disk"
    fileDlg.FileName = curMsg.GetFilename(bodyHandle)
    fileDlg.ShowSave
    If Not fileDlg.CancelError Then
        If curMsg.SaveBodyToFile(bodyHandle, fileDlg.FileName, False) = 0 Then
            MsgBox "Cannot write file"
        End If
    End If
End Sub
