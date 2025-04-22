VERSION 5.00
Begin VB.Form frmToolbarProps 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Toolbar Properties"
   ClientHeight    =   3045
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5085
   Icon            =   "frmToolbarProps.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3045
   ScaleWidth      =   5085
   ShowInTaskbar   =   0   'False
   Begin VB.ComboBox cboToolbarMsgWnd 
      Enabled         =   0   'False
      Height          =   315
      ItemData        =   "frmToolbarProps.frx":3452
      Left            =   2640
      List            =   "frmToolbarProps.frx":3460
      Style           =   2  'Dropdown List
      TabIndex        =   7
      Top             =   1830
      Width           =   2055
   End
   Begin VB.CheckBox OEToolbar 
      Caption         =   "Use &OE Toolbar"
      Height          =   375
      Left            =   360
      TabIndex        =   6
      Top             =   360
      Width           =   1815
   End
   Begin VB.CheckBox LargeBtns 
      Caption         =   "&Large Buttons"
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   840
      Value           =   1  'Checked
      Width           =   1815
   End
   Begin VB.CheckBox chkMsgWnd 
      Caption         =   "Create in &MsgWnd"
      Height          =   375
      Left            =   360
      TabIndex        =   4
      Top             =   1800
      Width           =   1815
   End
   Begin VB.CheckBox chkEnableTooltips 
      Caption         =   "Enable &Tooltips"
      Height          =   375
      Left            =   360
      TabIndex        =   3
      Top             =   1320
      Value           =   1  'Checked
      Width           =   1815
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2280
      TabIndex        =   0
      Top             =   2520
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   3720
      TabIndex        =   1
      Top             =   2520
      Width           =   1215
   End
   Begin VB.Frame Frame1 
      Height          =   2175
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4815
   End
End
Attribute VB_Name = "frmToolbarProps"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Dim okPressed As Boolean

Private Sub CancelButton_Click()
    okPressed = False
    Me.Hide
End Sub

Private Sub chkMsgWnd_Click()
    cboToolbarMsgWnd.Enabled = chkMsgWnd.Value
    'Debug.Print UBound(cboToolbarMsgWnd.List)
    cboToolbarMsgWnd.ListIndex = cboToolbarMsgWnd.ListCount - 1
End Sub

Private Sub Form_Load()
    frmFolders.SetTopMostWindow Me.hwnd, True
End Sub

Private Sub OEToolbar_Click()
    If Me.OEToolbar.Value Then
        Me.LargeBtns.Enabled = False
    Else
        Me.LargeBtns.Enabled = True
    End If
End Sub

Private Sub OKButton_Click()
    okPressed = True
    Me.Hide
End Sub

Public Function GetOKValue() As Boolean
    GetOKValue = okPressed
End Function
