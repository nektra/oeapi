VERSION 5.00
Begin VB.Form frmMessage 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message"
   ClientHeight    =   7860
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   7800
   Icon            =   "Message.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7860
   ScaleWidth      =   7800
   Begin VB.CommandButton delete 
      Caption         =   "Delete"
      Height          =   375
      Left            =   3840
      TabIndex        =   12
      Top             =   7440
      Width           =   1215
   End
   Begin VB.CommandButton close 
      Caption         =   "Close"
      Height          =   375
      Left            =   5160
      TabIndex        =   11
      Top             =   7440
      Width           =   1215
   End
   Begin VB.TextBox bodyStr 
      Height          =   3735
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   10
      Top             =   3600
      Width           =   7575
   End
   Begin VB.TextBox headerStr 
      Height          =   1455
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   9
      Top             =   2040
      Width           =   7575
   End
   Begin VB.CommandButton next 
      Caption         =   "Next"
      Height          =   375
      Left            =   6480
      TabIndex        =   0
      Top             =   7440
      Width           =   1215
   End
   Begin VB.Label norSubjectStr 
      Height          =   375
      Left            =   1080
      TabIndex        =   8
      Top             =   1440
      Width           =   6495
   End
   Begin VB.Label subjectStr 
      Height          =   375
      Left            =   960
      TabIndex        =   7
      Top             =   960
      Width           =   6615
   End
   Begin VB.Label Label4 
      Caption         =   "Normalized Subject:"
      Height          =   375
      Left            =   120
      TabIndex        =   6
      Top             =   1320
      Width           =   855
   End
   Begin VB.Label Label1 
      Caption         =   "Subject:"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   960
      Width           =   735
   End
   Begin VB.Label toStr 
      Height          =   255
      Left            =   600
      TabIndex        =   4
      Top             =   480
      Width           =   6975
   End
   Begin VB.Label fromStr 
      Height          =   255
      Left            =   720
      TabIndex        =   3
      Top             =   120
      Width           =   6975
   End
   Begin VB.Label Label3 
      Caption         =   "To:"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   480
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "From:"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   495
   End
End
Attribute VB_Name = "frmMessage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim buttonNext As Boolean
Dim buttonDelete As Boolean

Option Explicit


Private Sub close_Click()
    Me.Hide
    buttonNext = False
End Sub

Private Sub delete_Click()
    Me.Hide
    buttonDelete = True
End Sub

Private Sub Form_Load()
    Dim lr As Long
    lr = frmFolders.SetTopMostWindow(Me.hWnd, True)
    buttonNext = False
    buttonDelete = False
End Sub

Private Sub next_Click()
    Me.Hide
    buttonNext = True
End Sub

Public Function GetButtonNext() As Boolean
    GetButtonNext = buttonNext
End Function

Public Function GetButtonDelete() As Boolean
    GetButtonDelete = buttonDelete
End Function

