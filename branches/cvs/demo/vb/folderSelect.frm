VERSION 5.00
Begin VB.Form frmFolderSelect 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Select Folder"
   ClientHeight    =   4710
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   Icon            =   "folderSelect.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   4710
   ScaleWidth      =   6030
   Begin VB.ListBox folderList 
      Height          =   4350
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4455
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   4680
      TabIndex        =   1
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   4680
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "frmFolderSelect"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Dim selection As String

Private Sub CancelButton_Click()
    Me.Hide
    selection = ""

End Sub

Private Sub Form_Load()
    Dim lr As Long
    lr = frmFolders.SetTopMostWindow(Me.hwnd, True)
    RefreshFolders
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

Private Sub OKButton_Click()
    Me.Hide
    selection = Me.folderList.List(Me.folderList.ListIndex)
End Sub

Public Function GetSelection() As String
    GetSelection = selection
End Function

Public Function GetSelectionID() As Long
    Dim idStart As Integer
    Dim idEnd As Integer
    
    GetSelectionID = -1
    
    If selection <> "" Then
        idStart = InStr(selection, "(")
        idEnd = InStr(selection, ")")
        
        If idStart <> 0 And idEnd <> 0 Then
            GetSelectionID = Val(Mid(selection, idStart + 1, idEnd - idStart - 1))
        End If
    End If
    
End Function

