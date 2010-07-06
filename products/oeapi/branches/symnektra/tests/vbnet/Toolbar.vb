Imports OEAPIINITCOM
Imports OEAPI
Imports OESTORE
Imports System.Diagnostics


<ComClass(Toolbar.ClassId, Toolbar.InterfaceId, Toolbar.EventsId)> _
Public Class Toolbar

#Region "COM GUIDs"
    ' These  GUIDs provide the COM identity for this class 
    ' and its COM interfaces. If you change them, existing 
    ' clients will no longer be able to access the class.
    Public Const ClassId As String = "A6CD262A-7002-433C-86CB-D2747611319F"
    Public Const InterfaceId As String = "5DED78B3-CFED-4013-82DD-A4503923EECC"
    Public Const EventsId As String = "C7D88602-F3C6-4502-A3EA-2AE17A6D015A"
#End Region

    ' Private _dummy As Integer
    Private _init As OEAPIInit
    Private _oeapi As OEAPIObj
    
    ' A creatable COM class must have a Public Sub New() 
    ' with no parameters, otherwise, the class will not be 
    ' registered in the COM registry and cannot be created 
    ' via CreateObject.
    Public Sub New()
        MyBase.New()
        _init = New OEAPIInitClass
        AddHandler _init.OnInitOEAPI, AddressOf OnInitOEAPI
        AddHandler _init.OnShutdownOEAPI, AddressOf OnShutDownOEAPI
    End Sub

    Public Sub _Dummy()
        ' Dummy functions so VB .NET doesn't complain 
        ' about not having public members
    End Sub

    Private Sub OnInitOEAPI()
        Debug.WriteLine("+++OnInitOEAPI")
        Dim toolb As IOEToolbar

        _oeapi = New OEAPIObjClass

        toolb = _oeapi.CreateToolbar()
        toolb.CreateButton("a", "", "")
        toolb = Nothing

        AddHandler _oeapi.OnNewMessageInOutbox, AddressOf OnNewOutboxMessage
    End Sub

    Private Sub OnShutdownOEAPI()
        Debug.WriteLine("+++OnShutdownOEAPI")

        RemoveHandler _oeapi.OnNewMessageInOutbox, AddressOf OnNewOutboxMessage
        _oeapi.RemoveAllToolbars()
        _oeapi = Nothing

        RemoveHandler _init.OnShutdownOEAPI, AddressOf OnShutdownOEAPI
        RemoveHandler _init.OnInitOEAPI, AddressOf OnInitOEAPI

        ' Call GC twice, one only is not enough
        ' Event MS use this trick for Office
        ' http://msdn2.microsoft.com/en-us/library/aa679807(office.11).aspx
        GC.Collect()
        GC.WaitForPendingFinalizers()
        GC.Collect()
        GC.WaitForPendingFinalizers()
    End Sub

    Private Sub OnNewOutboxMessage(ByVal msgId As Integer)
        '
        Debug.WriteLine("+++OnNewOutboxMessage: " + msgId.ToString())
    End Sub
End Class


