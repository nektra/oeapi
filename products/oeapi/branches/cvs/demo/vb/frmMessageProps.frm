VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMessageProps 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Message Bodies"
   ClientHeight    =   7545
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6915
   Icon            =   "frmMessageProps.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7545
   ScaleWidth      =   6915
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox Picture1 
      BorderStyle     =   0  'None
      Height          =   375
      Left            =   3000
      ScaleHeight     =   375
      ScaleWidth      =   2295
      TabIndex        =   26
      Top             =   6480
      Width           =   2295
      Begin VB.OptionButton OtherBody 
         Caption         =   "Other"
         Height          =   255
         Left            =   1560
         TabIndex        =   27
         ToolTipText     =   "The text is in other format. Cannot be saved"
         Top             =   45
         Width           =   975
      End
      Begin VB.OptionButton HTMLBody 
         Caption         =   "HTML"
         Height          =   255
         Left            =   720
         TabIndex        =   28
         ToolTipText     =   "The text is in HTML format"
         Top             =   45
         Width           =   855
      End
      Begin VB.OptionButton PlainBody 
         Caption         =   "Plain"
         Height          =   255
         Left            =   0
         TabIndex        =   29
         ToolTipText     =   "The text is in plain format"
         Top             =   45
         Value           =   -1  'True
         Width           =   975
      End
   End
   Begin VB.TextBox PropValue 
      Height          =   855
      Left            =   2520
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   23
      ToolTipText     =   "Set the new text of the property"
      Top             =   2040
      Width           =   3135
   End
   Begin VB.ComboBox cbProp 
      Height          =   315
      Left            =   240
      TabIndex        =   22
      Text            =   "cbProp"
      ToolTipText     =   "Select the desire property name"
      Top             =   2280
      Width           =   2175
   End
   Begin VB.CommandButton SetProp 
      Caption         =   "Set Prop"
      Height          =   375
      Left            =   5760
      TabIndex        =   21
      ToolTipText     =   "Set the new property value"
      Top             =   2520
      Width           =   975
   End
   Begin VB.CommandButton RemoveProp 
      Caption         =   "Rem Prop"
      Height          =   375
      Left            =   5760
      TabIndex        =   20
      ToolTipText     =   "Remove property. Not all properties can be removed."
      Top             =   2040
      Width           =   975
   End
   Begin VB.Frame frmProperty 
      Caption         =   "Property"
      Height          =   1335
      Left            =   120
      TabIndex        =   19
      Top             =   1680
      Width           =   6735
      Begin VB.Label Label1 
         Caption         =   "Name"
         Height          =   255
         Left            =   120
         TabIndex        =   25
         Top             =   240
         Width           =   495
      End
      Begin VB.Label Label2 
         Caption         =   "Value"
         Height          =   255
         Left            =   2400
         TabIndex        =   24
         Top             =   120
         Width           =   495
      End
   End
   Begin VB.CommandButton HTMLBd 
      Caption         =   "HTML Body"
      Height          =   375
      Left            =   4200
      TabIndex        =   18
      Top             =   1200
      Width           =   1215
   End
   Begin VB.CommandButton Plainbd 
      Caption         =   "Plain Body"
      Height          =   375
      Left            =   5520
      TabIndex        =   17
      Top             =   1200
      Width           =   1215
   End
   Begin VB.CommandButton NextBody 
      Caption         =   "Next Body"
      Height          =   375
      Left            =   4200
      TabIndex        =   16
      ToolTipText     =   "Go to next body"
      Top             =   720
      Width           =   1215
   End
   Begin VB.CommandButton PrevBody 
      Caption         =   "Prev Body"
      Height          =   375
      Left            =   5520
      TabIndex        =   15
      ToolTipText     =   "Go to previous body"
      Top             =   720
      Width           =   1215
   End
   Begin VB.CommandButton ParentBody 
      Caption         =   "Parent Body"
      Height          =   375
      Left            =   4200
      TabIndex        =   14
      ToolTipText     =   "Go to parent body"
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton ChildBody 
      Caption         =   "Child Body"
      Height          =   375
      Left            =   5520
      TabIndex        =   13
      ToolTipText     =   "Go to first body child"
      Top             =   240
      Width           =   1215
   End
   Begin MSComDlg.CommonDialog fileDlg 
      Left            =   6600
      Top             =   5880
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton SetBodyFromFile 
      Caption         =   "Set Body From File"
      Height          =   375
      Left            =   1440
      TabIndex        =   12
      ToolTipText     =   "Set the content of the body from a file"
      Top             =   6480
      Width           =   1455
   End
   Begin VB.CommandButton SaveBodyToFile 
      Caption         =   "Save to File"
      Height          =   375
      Left            =   120
      TabIndex        =   11
      ToolTipText     =   "Save the content of the body to a file"
      Top             =   6480
      Width           =   1215
   End
   Begin VB.CommandButton SendMessage 
      Caption         =   "Send Message"
      Height          =   375
      Left            =   120
      TabIndex        =   10
      ToolTipText     =   "Send the Message"
      Top             =   7080
      Width           =   1215
   End
   Begin VB.CommandButton CloneMsg 
      Caption         =   "Clone Message"
      Height          =   375
      Left            =   1440
      TabIndex        =   9
      ToolTipText     =   "Create a exact copy of the message"
      Top             =   7080
      Width           =   1455
   End
   Begin VB.CommandButton DeleteBody 
      Caption         =   "Delete Body"
      Height          =   375
      Left            =   2760
      TabIndex        =   8
      ToolTipText     =   "Delete current body"
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton InsertNextBody 
      Caption         =   "Ins Next Body"
      Height          =   375
      Left            =   1440
      TabIndex        =   6
      ToolTipText     =   "Insert a body Next to current"
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton InsertChildBody 
      Caption         =   "Ins Child Body"
      Height          =   375
      Left            =   120
      TabIndex        =   5
      ToolTipText     =   "Insert a child to the body"
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton SaveBody 
      Caption         =   "Set Body Text"
      Height          =   375
      Left            =   5400
      TabIndex        =   4
      ToolTipText     =   "Save the modified text into the current body"
      Top             =   6480
      Width           =   1335
   End
   Begin VB.CommandButton CommitMessage 
      Caption         =   "Save Changes"
      Height          =   375
      Left            =   5400
      TabIndex        =   3
      ToolTipText     =   "Press this button to commit changes in the message"
      Top             =   7080
      Width           =   1335
   End
   Begin VB.TextBox bodyStr 
      Height          =   3135
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Top             =   3120
      Width           =   6735
   End
   Begin VB.TextBox BodyName 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   11274
         SubFormatType   =   1
      EndProperty
      Height          =   405
      Left            =   1200
      TabIndex        =   0
      ToolTipText     =   "Display name of the body"
      Top             =   120
      Width           =   2775
   End
   Begin VB.Frame Frame2 
      Caption         =   "Browse"
      Height          =   1695
      Left            =   4080
      TabIndex        =   7
      Top             =   0
      Width           =   2775
   End
   Begin VB.Line Line1 
      X1              =   0
      X2              =   6960
      Y1              =   6960
      Y2              =   6960
   End
   Begin VB.Label Label3 
      Caption         =   "Display Name"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "frmMessageProps"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim curMsg As OEMessage
Dim curBodyHandle As Long


Private Sub cbProp_Change()
    UpdateBodyProp
End Sub

Private Sub cbProp_Click()
    UpdateBodyProp
End Sub

Private Sub CloneMsg_Click()
    Dim newMsg As OEMessage
    Dim msgProps As New frmMessageProps
    
    Set newMsg = curMsg.Clone(curMsg.GetFolderID)
    
    If Not newMsg Is Nothing Then
        msgProps.SetMessage newMsg
        msgProps.Show vbModal
    End If
End Sub

Private Sub Form_Load()
    frmFolders.SetTopMostWindow Me.hWnd, True
End Sub

Public Sub SetMessage(msg As OEMessage)
    Dim bodyHandle As Long
    
    Set curMsg = msg
    
    ' set root body as current
    bodyHandle = curMsg.GetBodyHandle(0, OE_IBL_ROOT)
    SetCurrentBody bodyHandle
End Sub

Private Sub SetCurrentBody(bodyHandle As Long)
    Dim priCon As String
    Dim secCon As String
    Dim a As Variant
    
    If bodyHandle <> 0 Then
        a = curMsg.GetBodyText(bodyHandle)
        Me.bodyStr = curMsg.GetBodyText(bodyHandle)
        Me.BodyName.text = curMsg.GetBodyDisplayName(bodyHandle)
        curBodyHandle = bodyHandle
        
        priCon = curMsg.GetBodyPrimaryContentType(curBodyHandle)
        
        Me.OtherBody.Value = True
        
        If priCon = "text" Then
            secCon = curMsg.GetBodySecondaryContentType(curBodyHandle)
            If secCon = "html" Then
                Me.HTMLBody.Value = True
            Else
                If secCon = "plain" Then
                    Me.PlainBody.Value = True
                End If
            End If
        End If
        FillPropCombo
        
        UpdateBodyProp
    Else
        MsgBox "Invalid Body"
    End If
End Sub

Private Sub FillPropCombo()
    Dim propId&
    Dim propName As String
    Dim Index As Integer
    
    Me.cbProp.Clear
    Index = 0

    propName = curMsg.GetFirstBodyProp(curBodyHandle, propId)
    While propId <> -1
        Me.cbProp.AddItem propName, Index
        Me.cbProp.ItemData(Index) = propId

        propName = curMsg.GetNextBodyProp(propId)
    Wend
    
    Me.cbProp.ListIndex = 0
End Sub

Private Sub UpdateBodyProp()
    Dim propId As Long
    Dim PropValue As String
    Dim exist&
    
    propId = GetCurrentProp

    If curBodyHandle <> 0 Then
        If propId <> -1 Then
            PropValue = curMsg.GetBodyProp(curBodyHandle, propId, exist)
        Else
            PropValue = curMsg.GetBodyPropByName(curBodyHandle, Me.cbProp.text, exist)
        End If
        
        If exist <> 0 Then
            Me.PropValue.text = PropValue
        Else
            Me.PropValue.text = "invalid"
        End If
    End If
End Sub

Private Sub HTMLBd_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetHTMLBody
    SetCurrentBody bodyHandle
End Sub

Private Sub Plainbd_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetPlainBody
    SetCurrentBody bodyHandle
End Sub

Private Sub InsertChildBody_Click()
    curMsg.InsertBody curBodyHandle, OE_IBL_FIRST
End Sub

Private Sub InsertNextBody_Click()
    curMsg.InsertBody curBodyHandle, OE_IBL_NEXT
End Sub
Private Sub DeleteBody_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_NEXT)
    If bodyHandle = 0 Then
        bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_PREVIOUS)
    End If
    If bodyHandle = 0 Then
        bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_PARENT)
    End If
    
    curMsg.DeleteBody curBodyHandle, OE_DELETE_NOFLAGS
    
    SetCurrentBody bodyHandle
End Sub

Private Sub ParentBody_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_PARENT)
    SetCurrentBody bodyHandle
End Sub

Private Sub ChildBody_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_FIRST)
    SetCurrentBody bodyHandle
End Sub

Private Sub PrevBody_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_PREVIOUS)
    SetCurrentBody bodyHandle
End Sub

Private Sub NextBody_Click()
    Dim bodyHandle As Long
    
    bodyHandle = curMsg.GetBodyHandle(curBodyHandle, OE_IBL_NEXT)
    SetCurrentBody bodyHandle
End Sub

Private Sub RemoveProp_Click()
    Dim propId As Long

    If curBodyHandle <> 0 Then
        propId = GetCurrentProp

        If propId <> -1 Then
            curMsg.DeleteBodyProp curBodyHandle, propId
        Else
            curMsg.DeleteBodyPropByName curBodyHandle, Me.cbProp.text
        End If
    End If

    FillPropCombo
    UpdateBodyProp
End Sub

Private Sub SaveBody_Click()
    Dim priCon As String
    Dim secCon As String
    
    If Me.OtherBody.Value Then
        MsgBox "Demo application supports only Plain and HTML content types"
        Exit Sub
    End If
    
    priCon = "text"
    If Me.HTMLBody.Value Then
        secCon = "html"
    Else
        secCon = "plain"
    End If
    
    ' priCon and secCon can be empty strings and it will use
    ' the current content type of the body
    curMsg.SetBodyText curBodyHandle, Me.bodyStr.text, priCon, secCon
End Sub

Private Sub CommitMessage_Click()
    curMsg.Commit
End Sub

Private Sub SaveBodyToFile_Click()
    fileDlg.DialogTitle = "Select File"
    fileDlg.ShowSave
    If Not fileDlg.CancelError Then
        If curMsg.SaveBodyToFile(curBodyHandle, fileDlg.FileName, False) = 0 Then
            MsgBox "Cannot write body to file"
        End If
    End If
End Sub

Private Sub SendMessage_Click()
    curMsg.Send
End Sub

Private Sub SetBodyFromFile_Click()
    fileDlg.DialogTitle = "Select File"
    fileDlg.ShowOpen
    If Not fileDlg.CancelError Then
        If curMsg.SetBodyFromFile(curBodyHandle, fileDlg.FileName, "", "") = 0 Then
            MsgBox "Cannot write body to file"
        End If
    End If
End Sub

Private Sub SetProp_Click()
    Dim propId As Long
    Dim PropValue As String
    Dim propName As String
    Dim Index As Integer
    Dim exist&
    
    propId = GetCurrentProp

    If curBodyHandle <> 0 Then
        If propId <> -1 Then
            curMsg.SetBodyProp curBodyHandle, propId, Me.PropValue.text
        Else
            propName = Me.cbProp.text
            curMsg.SetBodyPropByName curBodyHandle, propName, Me.PropValue.text
            curMsg.GetBodyPropByName curBodyHandle, propName, exist

            If exist Then
                Index = Me.cbProp.ListCount
                Me.cbProp.AddItem propName, Index
                Me.cbProp.ItemData(Index) = propId
            End If
        End If
    End If
End Sub

Private Function GetCurrentProp() As Long
    Dim propId As Long
    Dim id As String
    Dim Index As Integer
    
    Index = Me.cbProp.ListIndex
    If Index = -1 Then
        GetCurrentProp = -1
    Else
        GetCurrentProp = Me.cbProp.ItemData(Index)
    End If
End Function

Private Sub SUBJECT_Click()
    UpdateBodyProp
End Sub

Private Sub TO_Click()
    UpdateBodyProp
End Sub

Private Sub XMAILER_Click()
    UpdateBodyProp
End Sub
Private Sub FROM_Click()
    UpdateBodyProp
End Sub

Private Sub FILENAME_Click()
    UpdateBodyProp
End Sub

Private Sub CNTTYPE_Click()
    UpdateBodyProp
End Sub

Private Sub RECEIVED_Click()
    UpdateBodyProp
End Sub

Private Sub RECVTIME_Click()
    UpdateBodyProp
End Sub

Private Sub REPLYTO_Click()
    UpdateBodyProp
End Sub

Private Sub RETRCPTTO_Click()
    UpdateBodyProp
End Sub

Private Sub ACCOUNT_Click()
    UpdateBodyProp
End Sub

Private Sub SENTTIME_Click()
    UpdateBodyProp
End Sub

Private Sub BCC_Click()
    UpdateBodyProp
End Sub

Private Sub CC_Click()
    UpdateBodyProp
End Sub

Private Sub DATE_Click()
    UpdateBodyProp
End Sub

