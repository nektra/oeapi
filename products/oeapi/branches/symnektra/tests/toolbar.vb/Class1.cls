VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Class1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Dim WithEvents mOEInit As OEAPIInit
Attribute mOEInit.VB_VarHelpID = -1
Dim WithEvents mOEAPIObj As OEAPIObj
Attribute mOEAPIObj.VB_VarHelpID = -1
Dim WithEvents mFolman As OEFolderManager
Attribute mFolman.VB_VarHelpID = -1
Dim WithEvents mFolder As OEFolder
Attribute mFolder.VB_VarHelpID = -1

Private Sub Class_Initialize()
    Set OEInit = New OEAPIInit
End Sub


Private Sub Class_Terminate()
    Set OEInit = Nothing
End Sub


Private Sub mFolder_OnMessageUnreadChange(ByVal msgId As Long, ByVal unread As Long)
    Debug.Print "Message unread change", msgId, unread
End Sub

Private Sub mFolder_OnNewMessage(ByVal msgId As Long)
    '
End Sub

Private Sub mOEInit_OnInitOEAPI()
    Debug.Print "Init OEAPI"
    Set mOEAPIObj = New OEAPIObj
    Set mFolman = New OEFolderManager
    Set mFolder = mFolman.GetInboxFolder
    ' Set mFolder = New OEFolder
End Sub


Private Sub mOEInit_OnShutdownOEAPI()
    Debug.Print "Shutdown OEAPI"
    Set mFolder = nothig
    Set mFolman = Nothing
    Set mOEAPIObj = Nothing
End Sub
