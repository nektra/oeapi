VERSION 5.00
Begin VB.Form frmCreateMsg 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "New Message"
   ClientHeight    =   7290
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   Icon            =   "frmCreateMsg.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7290
   ScaleWidth      =   6030
   Begin VB.TextBox Body 
      Height          =   5415
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   5
      Top             =   1320
      Width           =   5775
   End
   Begin VB.TextBox Subject 
      Height          =   285
      Left            =   960
      TabIndex        =   4
      Top             =   840
      Width           =   4935
   End
   Begin VB.TextBox To 
      Height          =   285
      Left            =   960
      TabIndex        =   3
      Top             =   480
      Width           =   4935
   End
   Begin VB.TextBox From 
      Height          =   315
      Left            =   960
      TabIndex        =   2
      Top             =   120
      Width           =   4935
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3360
      TabIndex        =   1
      Top             =   6840
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   4680
      TabIndex        =   0
      Top             =   6840
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "Subject:"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   840
      Width           =   735
   End
   Begin VB.Label Label2 
      Caption         =   "To:"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   480
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "From:"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   735
   End
End
Attribute VB_Name = "frmCreateMsg"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Dim cancel As Boolean

Private Sub CancelButton_Click()
    Me.Hide
    cancel = True
End Sub

Private Sub Form_Load()
    frmFolders.SetTopMostWindow Me.hwnd, True
    cancel = True
End Sub

Private Sub OKButton_Click()
    Me.Hide
    cancel = False
End Sub

Public Function GetCancel() As Boolean
    GetCancel = cancel
End Function

