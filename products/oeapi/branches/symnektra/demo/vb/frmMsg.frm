VERSION 5.00
Begin VB.Form frmMsg 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message Properties"
   ClientHeight    =   7380
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7800
   Icon            =   "frmMsg.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7380
   ScaleWidth      =   7800
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnSetCc 
      Caption         =   "Set Cc"
      Height          =   495
      Left            =   6840
      TabIndex        =   3
      Top             =   1440
      Width           =   855
   End
   Begin VB.TextBox tbCc 
      Height          =   375
      Left            =   1200
      TabIndex        =   2
      Top             =   1440
      Width           =   5535
   End
   Begin VB.CommandButton btnSetTo 
      Caption         =   "Set To"
      Height          =   495
      Left            =   6840
      TabIndex        =   1
      Top             =   840
      Width           =   855
   End
   Begin VB.TextBox tbTo 
      Height          =   375
      Left            =   1200
      TabIndex        =   0
      Top             =   840
      Width           =   5535
   End
   Begin VB.CommandButton btnSetSubject 
      Caption         =   "Set Subject"
      Height          =   495
      Left            =   6840
      TabIndex        =   5
      Top             =   2040
      Width           =   855
   End
   Begin VB.CommandButton btnSetBody 
      Caption         =   "Set Body"
      Height          =   615
      Left            =   6840
      TabIndex        =   7
      Top             =   4080
      Width           =   855
   End
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Enabled         =   0   'False
      Height          =   495
      Left            =   1320
      TabIndex        =   9
      Top             =   120
      Width           =   975
   End
   Begin VB.CommandButton btnSendMsg 
      Caption         =   "Send Msg"
      Height          =   495
      Left            =   120
      TabIndex        =   8
      Top             =   120
      Width           =   975
   End
   Begin VB.TextBox tbBody 
      Height          =   4695
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   6
      Top             =   2520
      Width           =   6615
   End
   Begin VB.TextBox tbSubject 
      Height          =   405
      Left            =   1200
      TabIndex        =   4
      Top             =   2040
      Width           =   5535
   End
   Begin VB.Label Label2 
      Caption         =   "Cc:"
      Height          =   495
      Left            =   120
      TabIndex        =   12
      Top             =   1440
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "To:"
      Height          =   375
      Left            =   120
      TabIndex        =   11
      Top             =   840
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "Subject:"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   2160
      Width           =   855
   End
End
Attribute VB_Name = "frmMsg"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim msgWnd As OEMsgWnd

Public Sub SetMsgWnd(wnd As OEMsgWnd)
    Dim text As String

    Set msgWnd = wnd
    
    Select Case msgWnd.GetWndStyle
        Case OE_MSG_DETAIL_WND
            btnSendMsg.Enabled = False
        Case OE_CURRENT_MSG_WND
            btnSetTo.Enabled = False
            btnSetCc.Enabled = False
            btnSetSubject.Enabled = False
            tbTo.Enabled = False
            tbCc.Enabled = False
            tbSubject.Enabled = False
            btnSendMsg.Enabled = False
        Case OE_EMPTY_MSG_WND Or OE_MSG_INVALID Or OE_OTHER_WND
            btnSetTo.Enabled = False
            btnSetCc.Enabled = False
            btnSetSubject.Enabled = False
            tbTo.Enabled = False
            tbCc.Enabled = False
            tbSubject.Enabled = False
            tbBody.Enabled = False
            btnSendMsg.Enabled = False
    End Select

    tbTo = msgWnd.GetTo
    tbCc = msgWnd.GetCc
    tbSubject = msgWnd.GetSubject
    tbBody = msgWnd.GetBody
End Sub

Private Sub btnCancel_Click()
    ' Cancel the send process started by the user
    msgWnd.CancelSend
    Unload Me
End Sub

Private Sub btnSendMsg_Click()
    ' send the mail and destroy the form
    msgWnd.SendMail
    Unload Me
End Sub

Private Sub btnSetTo_Click()
    ' Set new values of the To
    msgWnd.SetTo Me.tbTo
End Sub

Private Sub btnSetCc_Click()
    ' Set new values of the Cc
    msgWnd.SetCc Me.tbCc
End Sub

Private Sub btnSetSubject_Click()
    ' Set new values of the subject
    msgWnd.SetSubject Me.tbSubject
End Sub

Private Sub btnSetBody_Click()
    ' Set new values of the body using the SetBodyHTML method
    msgWnd.SetBodyHTML Me.tbBody

    ' Set new values of the body using the SetBody method
'    MsgWnd.SetBody Me.tbBody
End Sub

Private Sub Form_Load()
    ' Keep the form always on top
    frmFolders.SetTopMostWindow Me.hWnd, True
End Sub
