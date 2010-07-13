VERSION 5.00
Begin VB.Form frmMsgWnds 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message Windows"
   ClientHeight    =   3030
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4680
   Icon            =   "frmMsgWnds.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3030
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnRefresh 
      Caption         =   "Refresh"
      Height          =   375
      Left            =   3480
      TabIndex        =   2
      Top             =   720
      Width           =   1095
   End
   Begin VB.CommandButton btnView 
      Caption         =   "View"
      Height          =   375
      Left            =   3480
      TabIndex        =   1
      Top             =   120
      Width           =   1095
   End
   Begin VB.ListBox lbMsgWnds 
      Height          =   2790
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3255
   End
End
Attribute VB_Name = "frmMsgWnds"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents Oeapi As OEAPIObj
Attribute Oeapi.VB_VarHelpID = -1
Private WithEvents MsgBtn As OEButton
Attribute MsgBtn.VB_VarHelpID = -1

Private MsgToolbar As OEToolbar


Private Sub Form_Load()
    Dim msgWndId As Long
    Dim imgNormal As String
    Dim imgMouseOver As String
    
    Set Oeapi = New OEAPIObj

    ' Use OE default toolbar to create a button
    Set MsgToolbar = Oeapi.GetOEToolbarInMsgWnd(OE_MSG_SEND_WND Or OE_MSG_DETAIL_WND)

    ' create a toolbar separator
    MsgToolbar.CreateSeparator

    ' or create your own toolbar
'       Set msgToolbar = Oeapi.CreateToolbarInMsgWnd(OE_MSG_SEND_WND)
'       msgToolbar.SetLargeButtons (True)

    Set MsgBtn = New OEButton

    imgNormal = frmFolders.Path + "\images\img_normal.bmp"
    imgMouseOver = frmFolders.Path + "\images\img_mouseover.bmp"
    MsgBtn.Create MsgToolbar.GetID(), "Modify", imgNormal, imgMouseOver
    
    MsgBtn.CreateSubButton "Sub1", imgNormal, imgMouseOver
    
    frmFolders.SetTopMostWindow Me.hwnd, True
    RefreshMsgWnds
End Sub

Private Sub Form_Unload(cancel As Integer)
    MsgToolbar.Destroy
    
    Set MsgBtn = Nothing
    Set MsgToolbar = Nothing
End Sub

Private Sub RefreshMsgWnds()
    Dim msgWndEntry As String
    Dim msgWnd As OEMsgWnd
    Dim activeMsgWndId As Long
    Dim msgWndId As Long
    
    lbMsgWnds.Clear
    
    activeMsgWndId = Oeapi.GetActiveMsgWndID
    
    msgWndId = Oeapi.GetFirstMsgWndID
    While msgWndId <> -1
        Set msgWnd = Oeapi.GetMsgWnd(msgWndId)
        If Not msgWnd Is Nothing Then
            msgWndEntry = msgWndId & " "

            Select Case msgWnd.GetWndStyle
                Case OE_MSG_INVALID
                    msgWndEntry = msgWndEntry & "OE_MSG_INVALID"
                Case OE_MSG_DETAIL_WND
                    msgWndEntry = msgWndEntry & "OE_MSG_DETAIL_WND"
                Case OE_MSG_SEND_WND
                    msgWndEntry = msgWndEntry & "OE_MSG_SEND_WND"
                Case OE_CURRENT_MSG_WND
                    msgWndEntry = msgWndEntry & "OE_CURRENT_MSG_WND"
                Case OE_EMPTY_MSG_WND
                    msgWndEntry = msgWndEntry & "OE_EMPTY_MSG_WND"
                Case OE_OTHER_WND
                    msgWndEntry = msgWndEntry & "OE_OTHER_WND"
            End Select
            
            If activeMsgWndId = msgWndId Then
                msgWndEntry = msgWndEntry & " " & "ACTIVE"
            End If
            
            lbMsgWnds.AddItem msgWndEntry
        End If
        
        msgWndId = Oeapi.GetNextMsgWndID
    Wend
End Sub

Private Function GetSelMsgWndId() As Long
    GetSelMsgWndId = GetMsgWndId(lbMsgWnds.ListIndex)
End Function

Private Function GetMsgWndId(Index As Integer) As Long
    Dim id As String
    Dim sel As String
    
    sel = Me.lbMsgWnds.List(Index)
    If sel = "" Then
        GetMsgWndId = -1
    Else
        id = Left(sel, InStr(1, sel, " ", 1))
        GetMsgWndId = CLng(id)
    End If
End Function

Private Sub RemoveFromList(msgWndId As Long)
    Dim sel As String
    Dim i As Integer
    
    For i = 1 To lbMsgWnds.ListCount
        If msgWndId = GetMsgWndId(i) Then
            lbMsgWnds.RemoveItem i
            Exit Sub
        End If
    Next i
End Sub

Private Sub Oeapi_OnNewMsgWnd(ByVal msgWndId As Long)
    RefreshMsgWnds
End Sub

Private Sub Oeapi_OnMsgWndClosed(ByVal msgWndId As Long, ByVal isMainWindow As Long)
    Dim msgWnd As OEMsgWnd
    Dim wndStyle As tagWNDSTYLE
    Dim msgFrm As New frmMsg

    If isMainWindow = 0 Then
        Set msgWnd = Oeapi.GetMsgWnd(msgWndId)
        If Not msgWnd Is Nothing Then
            wndStyle = msgWnd.GetWndStyle

            msgFrm.SetMsgWnd msgWnd
            msgFrm.Show vbModal
        End If
    End If
    
    RemoveFromList msgWndId
End Sub

Private Sub btnRefresh_Click()
    RefreshMsgWnds
End Sub

Private Sub btnView_Click()
    Dim msgWnd As OEMsgWnd
    Dim msgFrm As New frmMsg
    Dim msgWndId As Long

    msgWndId = GetSelMsgWndId
    If msgWndId <> -1 Then
        Set msgWnd = Oeapi.GetMsgWnd(msgWndId)
        If Not msgWnd Is Nothing Then
            msgFrm.SetMsgWnd msgWnd
            msgFrm.Show vbModal
        End If
    End If
End Sub

Private Sub Oeapi_OnSendButtonMsgWndClicked(ByVal msgWndId As Long)
    Dim msgWnd As OEMsgWnd
    Dim msgFrm As New frmMsg

    Set msgWnd = Oeapi.GetMsgWnd(msgWndId)
    msgFrm.SetMsgWnd msgWnd

    ' enable the cancel Send process and disable the 'Send' button
    msgFrm.btnSendMsg.Enabled = False
    msgFrm.btnCancel.Enabled = True

    msgFrm.Show vbModal
End Sub

Private Sub MsgBtn_OnMsgWndClick(ByVal msgWndId As Long)
    Dim msgWnd As OEMsgWnd
    Dim msgFrm As New frmMsg

    Set msgWnd = Oeapi.GetMsgWnd(msgWndId)
    msgFrm.SetMsgWnd msgWnd
    msgFrm.Show vbModal
End Sub

